#include "ConsoleRenderer.h"


// simple raycast algorithm, returns float distance from player to object
ConsoleRenderer::ConsoleRenderer() {

	screenBuffer.resize(RENDER_HEIGHT);	// vector size, not string one
	for (auto& line : screenBuffer)
		line.resize(RENDER_WIDTH, ' '); // resize and clear strings

	// setup console handle
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// hide cursor when console is active
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

}


// Chooses symbol color depending on its shape and sets color to it
// predefine with 1 not to use when not written in other code
char ConsoleRenderer::getObjectSymb(float distance) {

	static const float depth = RaycastEngine::DEPTH;

	static const char symbs[] = { ' ', '#', 'X', 'O', 'x', '-', '.' };				// nothing as 0
	static constexpr int symbArrSize = static_cast<int>((sizeof(symbs) / (sizeof(symbs[0]))));	// to know it when compiled
	static const float distLimits[] = {
		depth / 4.0f,					// too short dist
		depth / 3.5f,
		depth / 3.0f,
		depth / 2.5f,
		depth / 2.0f,
		depth							// too far dist
	};


	for (int i = 1; i < symbArrSize - 1; ++i)
		if (distance <= distLimits[i])
			return symbs[i];

	return symbs[0];	// return ' ' if nothing
}


void ConsoleRenderer::setObjectColor(const char symb, const int heightValue) { // predefine with 1 not to use when not written in other code

	switch (symb) {
		// in-screen walls displaying colors
	case '#':
		Render::Utils::SetTextColor(Render::Objects::Colors::BrightWhite, hConsole);
		break;
	case 'X':
		if (!heightValue)	// if this is first line (y=0) where player info located
			Render::Utils::SetTextColor(Render::Objects::Colors::BrightGreen, hConsole);
		else // 'X' as wall symbol
			Render::Utils::SetTextColor(Render::Objects::Colors::White, hConsole); // white
		break;
	case 'O':
		Render::Utils::SetTextColor(Render::Objects::Colors::RedGreen, hConsole);
		break;
	case 'x':
		Render::Utils::SetTextColor(Render::Objects::Colors::RedBlue, hConsole);
		break;
	case '-':
		Render::Utils::SetTextColor(Render::Objects::Colors::Blue, hConsole);
		break;
	case '.':
		Render::Utils::setDefaultColor(hConsole);
		break;
	case '@':
		Render::Utils::SetTextColor(Render::Objects::Colors::BrightRed, hConsole);
		break;
	default:

		if (isalpha(symb) || isdigit(symb))		// zero-line player info, recolor only text/nums
			Render::Utils::SetTextColor(Render::Objects::Colors::BrightGreen, hConsole);
		else Render::Utils::setDefaultColor(hConsole);
		break;
	}

}


// Main rendering function with all renderable elements implemented
// bool useDDA enables/disables castRay_DDA
void ConsoleRenderer::render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle, bool useDDA) {

	// removed cls
	// system("cls");

	// render every col on the screen to make walls
	for (int x = 0; x < RENDER_WIDTH; ++x) {

		float rayAngle = (playerAngle - RaycastEngine::FOV / 2.0f) + ((float)x / (float)RENDER_WIDTH) * RaycastEngine::FOV;

		// casting ray to return wall distance, with algorithm choosing now
		float distance = useDDA ? raycast.castRay_dda(map, playerCoord, rayAngle) : raycast.castRay_stepped(map, playerCoord, rayAngle);

		// fix fish eye effect
		distance *= cosf(rayAngle - playerAngle);

		// height of wall is inverted value of the distance
		int wallHeight = (int)(RENDER_HEIGHT / distance);

		// because top coords on the screen are (0,0), use the by subtracting the height of the wall from the render height, 
		// we get the top point from which to draw the wall on the screen
		int wallTop = (RENDER_HEIGHT / 2) - (wallHeight / 2);
		int wallBottom = (RENDER_HEIGHT / 2) + (wallHeight / 2); 		// the same logic but width bottom point

		for (int y = 0; y < RENDER_HEIGHT; ++y) {
			int screenIndex = y * RENDER_WIDTH + x;	// scale coord to screen size

			if (y < wallTop) 											// upper the wall
				screenBuffer[y][x] = Render::Objects::PATH;
			else if (y >= wallTop && y <= wallBottom)					// in-wall
				screenBuffer[y][x] = getObjectSymb(distance);
			else 														// under the wall
				screenBuffer[y][x] = Render::Objects::PATH;
		}

	}

	// another renderable things here
	renderPlayerInfo_buffered(playerCoord, playerAngle);
	renderMiniMap(map, playerCoord);

	// draw final buffer to the screen (render -> screen)
	displayScreen();
}


void ConsoleRenderer::renderPlayerInfo_buffered(const fcoord_t& playerCoord, float playerAngle) {

	char infoBuf[64];
	sprintf_s(infoBuf, "X:%.2f Y:%0.2f Angle:%0.2f", playerCoord.x, playerCoord.y, playerAngle);	// setup buffer

	std::string info = infoBuf;
	if (info.length() > RENDER_WIDTH) info.resize(RENDER_WIDTH);

	for (int i = 0; i < strlen(infoBuf) && i < RENDER_WIDTH; ++i)
		screenBuffer[0][i] = info[i];
}


void ConsoleRenderer::renderMiniMap(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord) {
	
	int mapHeight = static_cast<int>(map.size());
	int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;
	
	// get miniMapSize depending on real map sizes (array sizes) and max MINIMAP_SIZE
	int miniMapSize = MathUtils::Func::_min(MINIMAP_SIZE, MathUtils::Func::_max(mapHeight, mapWidth));

	coord_t offset = { RENDER_WIDTH - miniMapSize - 1, 1 };

	// minimap border
	for (int i = 0; i < miniMapSize + 2; ++i)
		if (offset.y + i >= 0 && offset.y + i < RENDER_HEIGHT) {

			if (offset.x - 1 >= 0)
				screenBuffer[offset.y + i][offset.x - 1] = '|';

			if (offset.x + miniMapSize < RENDER_WIDTH)
				screenBuffer[offset.y + i][offset.x + miniMapSize] = '|';
		}


	for (int i = -1; i <= miniMapSize; ++i)
		if (offset.x + i >= 0 && offset.x + i < RENDER_WIDTH) {

			if (offset.y - 1 >= 0)
				screenBuffer[offset.y - 1][offset.x + i] = '-';

			if (offset.y + miniMapSize < RENDER_HEIGHT)
				screenBuffer[offset.y + miniMapSize][offset.x + i] = '_';
		}

	// info inside of minimap
	for (int y = 0; y < mapHeight && y < miniMapSize; ++y)
		for (int x = 0; x < mapWidth && x < miniMapSize; ++x) {

			int screenX = offset.x + x;
			int screenY = offset.y + y;

			if (screenX >= 0 && screenX < RENDER_WIDTH && screenY >= 0 && screenY < RENDER_HEIGHT) {

				screenBuffer[screenY][screenX] = map[y][x];

				// player pos
				if ((int)playerCoord.x == x && (int)playerCoord.y == y)
					screenBuffer[screenY][screenX] = '@';
			}
		}

}


// normalize angle to [0; 2pi]
void ConsoleRenderer::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}

// Clears screenBuffer
void ConsoleRenderer::clearScreenBuffer() {

	for (auto& line : screenBuffer)
		std::fill(line.begin(), line.end(), ' ');
}


// display buffer through console output and also define elements colors
void ConsoleRenderer::displayScreen() {

	//clearScreenBuffer();

	Render::Utils::MoveToXY(0, 0, hConsole);
	for (int y = 0; y < RENDER_HEIGHT; ++y) {
		Render::Utils::MoveToXY(0, y, hConsole);		// move to start of every Y line

		for (int x = 0; x < RENDER_WIDTH; ++x) {

			char symb = screenBuffer[y][x];
			setObjectColor(symb, y);

			// display symbol after coloring
			std::cout << symb;
		}
	}

	Render::Utils::setDefaultColor();
	std::cout.flush(); // removed cls so flush out buffer directly
}
