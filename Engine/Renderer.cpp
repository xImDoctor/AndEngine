#include "Renderer.h"


// simple raycast algorithm, returns float distance from player to object
Renderer::Renderer() {

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


// Get drawing symbol for wall based on its distance
float Renderer::castRay_stepped(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle) {

	int mapHeight = static_cast<int>(map.size());
	int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

	// angle normalization
	normalizeAngle(rayAngle);

	fcoord_t rayCoord = playerCoord;	// cast rays from player pos

	fcoord_t rayDirection;
	rayDirection.x = cosf(rayAngle);
	rayDirection.y = sinf(rayAngle);

	float distance = 0.0f;
	float stepSize = 0.01f;  // ray step size

	bool isWallHit = false;

	while (!isWallHit && distance < depth) {
		distance += stepSize;

		// ray new position
		rayCoord.x = playerCoord.x + rayDirection.x * distance;
		rayCoord.y = playerCoord.y + rayDirection.y * distance;

		// check map border
		if (rayCoord.x < 0 || rayCoord.x >= mapWidth || rayCoord.y < 0 || rayCoord.y >= mapHeight) {
			isWallHit = true;
			distance = depth;
		}
		else {
			// check if wall is hit
			coord_t mapCoord = { static_cast<int>(rayCoord.x), static_cast<int>(rayCoord.y) };
			if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
				isWallHit = true;
		}

	}

	return distance;
}


// DDA version of raycast
float Renderer::castRay_dda(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle) {

	int mapHeight = static_cast<int>(map.size());
	int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

	if (!mapHeight || !mapWidth)
		return depth;


	normalizeAngle(rayAngle);

	fcoord_t rayCoord = playerCoord;
	fcoord_t rayDirection = { cosf(rayAngle), sinf(rayAngle) };

	// which box of the map (grid) we are in; int coord struct
	coord_t mapCoord = { static_cast<int>(rayCoord.x), static_cast<int>(rayCoord.y) };

	// border check as at the stepped alg
	if (mapCoord.x < 0 || mapCoord.x >= mapWidth || mapCoord.y < 0 || mapCoord.y >= mapHeight)
		return depth;

	// we have directly hit a wall
	if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
		return 0.0f;

	// init max  (limited) float value for alg
	constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();

	// delta distances of alg
	fcoord_t deltaDistance;
	deltaDistance.x = (rayDirection.x == 0.0f) ? FLOAT_INFINITY : std::fabsf(1.0f / rayDirection.x);
	deltaDistance.y = (rayDirection.y == 0.0f) ? FLOAT_INFINITY : std::fabsf(1.0f / rayDirection.y);

	coord_t stepCoord;			// int coords of each step for grid cells
	fcoord_t sideDistance;	// side distance coord

	if (rayDirection.x < 0.0f) {
		stepCoord.x = -1;
		sideDistance.x = rayCoord.x - mapCoord.x * deltaDistance.x;
	}
	else {
		stepCoord.x = 1;
		sideDistance.x = mapCoord.x + 1 - rayCoord.x * deltaDistance.x;
	}

	if (rayDirection.y < 0.0f) {
		stepCoord.y = -1;
		sideDistance.y = rayCoord.y - mapCoord.y * deltaDistance.y;
	}
	else {
		stepCoord.y = 1;
		sideDistance.y = mapCoord.y + 1 - rayCoord.y * deltaDistance.y;
	}

	bool isWallHit = false;
	int side = 0;	// if 0 then hit vertical side (x), if 1 then hit horizontal side (y)

	while (!isWallHit) {

		if (sideDistance.x < sideDistance.y) {	// vertical side
			sideDistance.x += deltaDistance.x;
			mapCoord.x += stepCoord.x;
			side = 0;
		}
		else {									// else horiz one
			sideDistance.y += deltaDistance.y;
			mapCoord.y += stepCoord.y;
			side = 1;
		}

		// check bounds again
		if (mapCoord.x < 0 || mapCoord.x >= mapWidth || mapCoord.y < 0 || mapCoord.y >= mapHeight)
			return depth;

		// if coords crosses with wall - we hit it
		if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
			isWallHit = true;

	}

	// calc perpendicular distance (projected on camera direction)
	float perpDistance;

	if (side)	// 1, horiz line
		perpDistance = (mapCoord.y - playerCoord.y + (1 - stepCoord.y) / 2.0f) / rayDirection.y;
	else		// vertical line
		perpDistance = (mapCoord.x - playerCoord.x + (1 - stepCoord.x) / 2.0f) / rayDirection.x;

	// if distance bigger then max distance, return max one
	if (perpDistance > depth)
		return depth;

	return perpDistance;
}


// Chooses symbol color depending on its shape and sets color to it
// predefine with 1 not to use when not written in other code
char Renderer::getObjectSymb(float distance) {

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


void Renderer::setObjectColor(const char symb, const int heightValue) { // predefine with 1 not to use when not written in other code

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
void Renderer::render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle, bool useDDA) {

	// removed cls
	// system("cls");

	// render every col on the screen to make walls
	for (int x = 0; x < RENDER_WIDTH; ++x) {

		float rayAngle = (playerAngle - FOV / 2.0f) + ((float)x / (float)RENDER_WIDTH) * FOV;

		// casting ray to return wall distance, with algorithm choosing now
		float distance = useDDA ? castRay_dda(map, playerCoord, rayAngle) : castRay_stepped(map, playerCoord, rayAngle);

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
	renderPlayerInfo(playerCoord, playerAngle);
	renderMiniMap(map, playerCoord);

	// draw final buffer to the screen (render -> screen)
	displayScreen();
}


void Renderer::renderPlayerInfo(const fcoord_t& playerCoord, float playerAngle) {

	char infoBuf[64];
	sprintf_s(infoBuf, "X:%.2f Y:%0.2f Angle:%0.2f", playerCoord.x, playerCoord.y, playerAngle);	// setup buffer

	std::string info = infoBuf;
	if (info.length() > RENDER_WIDTH) info.resize(RENDER_WIDTH);

	for (int i = 0; i < strlen(infoBuf) && i < RENDER_WIDTH; ++i)
		screenBuffer[0][i] = info[i];
}


void Renderer::renderMiniMap(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord) {
	
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
void Renderer::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}

// Clears screenBuffer
void Renderer::clearScreenBuffer() {

	for (auto& line : screenBuffer)
		std::fill(line.begin(), line.end(), ' ');
}


// display buffer through console output and also define elements colors
void Renderer::displayScreen() {

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
