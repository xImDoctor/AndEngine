#include "Engine.h"


// Game cycle running
void Engine::run(bool use_DDA_render) {

	system("cls");			// clear window before game drawing

	while (isRunning) {

		handleInput();
		// stop render when Esc pressed
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			isRunning = false;


		renderer.render(map, playerCoord, playerAngle, use_DDA_render);

		// sleep time with delay to stabilize frames (~FPS)
		Sleep(renderer.getRenderDelay());
	}
}


// Generate in-scene map randomly
std::vector<std::vector<char>> Engine::generateMap() {

	std::vector<std::vector<char>> newMap(MAP_HEIGHT, std::vector<char>(MAP_WIDTH));

	for (int i = 0; i < MAP_HEIGHT; ++i) {

		for (int j = 0; j < MAP_WIDTH; ++j) {

			if (!i || !j || j == MAP_WIDTH - 1 || i == MAP_HEIGHT - 1)
				newMap[i][j] = Render::Objects::WALL;	// coz dimentions are presized now
			else
				newMap[i][j] = Render::Objects::PATH;
		}
	}

	coord_t coords = { 2, 2 };	// border is already made, and second cell used for player spawn right now

	// randomly spawn walls
	for (int i = coords.x; i < MAP_HEIGHT - 2; ++i)
		for (int j = coords.y; j < MAP_WIDTH - 2; ++j) {
			coord_t spawnCoords = { i , j };

			//if (RenderObjects::isWallSpawnable(spawnCoords, MAP_HEIGHT, MAP_WIDTH))
			if (SpawnUtils::isWallPercentSpawnable(spawnCoords, 30))		// 40% by default
				newMap[i][j] = Render::Objects::WALL;
		}

	newMap[coords.x][coords.y] = Render::Objects::PATH;

	return newMap;
}


// User input processing
void Engine::handleInput() {

	// rotate to A-D
	// move forward-backward to W-S
	if (GetAsyncKeyState('A') & 0x8000)
		playerAngle -= ROTATION_SPEED;
	else if (GetAsyncKeyState('D') & 0x8000)
		playerAngle += ROTATION_SPEED;
	else {

		fcoord_t newPlayerCoord = playerCoord;

		if (GetAsyncKeyState('W') & 0x8000) {

			newPlayerCoord.x = playerCoord.x + cosf(playerAngle) * MOVEMENT_SPEED;
			newPlayerCoord.y = playerCoord.y + sinf(playerAngle) * MOVEMENT_SPEED;
		}
		else if (GetAsyncKeyState('S') & 0x8000) {

			newPlayerCoord.x = playerCoord.x - cosf(playerAngle) * MOVEMENT_SPEED;
			newPlayerCoord.y = playerCoord.y - sinf(playerAngle) * MOVEMENT_SPEED;
		}

		if (map[(int)newPlayerCoord.y][(int)newPlayerCoord.x] == Render::Objects::PATH)
			playerCoord = newPlayerCoord;

		normalizeAngle(playerAngle);
	}
}


// normalize angle to [0; 2pi]
void Engine::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}