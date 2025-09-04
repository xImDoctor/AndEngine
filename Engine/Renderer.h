#pragma once

#include <Windows.h>
#include <vector>

#include <cstdlib>
#include <ctime>
#include <cmath>

#include <cstdio>
#include <iostream>

// simplified (shorten Pi defined), not const from math.h
#define rendPI 3.14159f

// todo: move map creation to Game class (but right now it is simplified)
// todo: choose drawing color for wall based on its disntace - getWallColor
// HAVE TO DO A LOT OF REFACTORING!


struct coord_t {
	int x, y;

	// overload for == operator to use coord_t directly in the conditions
	friend bool operator==(const coord_t& coords1, const coord_t& coords2) {
		if (coords1.x == coords2.x && coords1.y == coords2.y)
			return true;

		return false;
	}

	friend bool operator!=(const coord_t& coords1, const coord_t& coords2) { 
		return !(coords1 == coords2);
	}
};


struct fcoord_t {
	float x, y;

	// overload for == operator to use fcoord_t directly in the conditions
	friend bool operator==(const fcoord_t& coords1, const fcoord_t& coords2) {
		if (coords1.x == coords2.x && coords1.y == coords2.y)
			return true;

		return false;
	}

	friend bool operator!=(const fcoord_t& coords1, const fcoord_t& coords2) {
		return !(coords1 == coords2);
	}
};


namespace RenderObjects {

	enum objectTypes { PATH = ' ', WALL = '#' };


	// to make material and other params for objects (empty for now)
	struct renderer_object_t {

	};

	namespace Utils {

		// spawn wall if taken coords == random coords width got from area
		// use srand() on the game initialization, this is not the task of Renderer
		// todo: move map creation to Game class (but right now it is simplified)
		bool isWallSpawnable(const coord_t& coords, const int areaHeight, const int areaWidth) {

			coord_t spawnCoords;
			spawnCoords.x = rand() % areaHeight;
			spawnCoords.y = rand() % areaWidth;

			if (coords == spawnCoords)
				return true;

			return false;
		}

		bool isWallPercentSpawnable(const coord_t& coords, int spawnPercent = 40) {

			if ((rand() % 100 + 1) <= spawnPercent)
				return true;

			return false;
		}
	}
}

// try to make base raytracing system - Console Raycaster
class Renderer {

	// render window size
	static const int RENDER_HEIGHT = 120;
	static const int RENDER_WIDTH = 40;

	// array size
	static const int MAP_HEIGHT = 16;
	static const int MAP_WIDTH = 16;

	// array to store game map
	std::vector<std::vector<char>> map;		// MAP_HEIGHT x MAP_WIDTH


	fcoord_t playerCoord;			// x, y
	float playerAngle;

	float fov = rendPI / 4.0f;		// field of view, pi/4
	float depth = 16.0f;			// max render distance


	// console
	HANDLE hConsole;
	CHAR_INFO* screen;

public:

	Renderer() {

		srand(static_cast<unsigned int>(time(NULL)));

		playerCoord = { 2.0f, 2.0f };
		playerAngle = 0.0f;

		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole);

		screen = new CHAR_INFO[RENDER_HEIGHT * RENDER_WIDTH];
		
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		map = generateMap();

	}

	~Renderer() {
		delete[] screen;
		CloseHandle(hConsole);
	}


	// show created/choosen map
	void renderMap() const {

		for (const auto& row : map) {
			for (const auto cell : row)
				std::cout << cell;
			
			std::cout << std::endl;
		}
	}


	// simple raycast algorithm, returns float distance from player to object
	float castRay(float rayAngle) {
			
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
			if (rayCoord.x < 0 || rayCoord.x >= MAP_WIDTH || rayCoord.y < 0 || rayCoord.y >= MAP_HEIGHT) {
				isWallHit = true;
				distance = depth;
			}
			else {
				// check if wall is hit
				coord_t mapCoord = { (int)rayCoord.x, (int)rayCoord.y };
				if (map[mapCoord.y][mapCoord.x] == RenderObjects::WALL)
					isWallHit = true;
			}

		}

		return distance;
	}


	// choose drawing symbol for wall based on its distance
	char getWallSymb(float distance) {

		if (distance <= depth / 4.0f) return '#';		// too short
		else if (distance <= depth / 3.0f) return 'X';
		else if (distance <= depth / 2.0f) return 'x';
		else if (distance <= depth)	return '.';			// too far
		 
		return ' '; // nothing in the view
	}

	// todo:: choose drawing color for wall based on its distance
	WORD getWallColor(float dinstance) {

		return 0;
	}


	void render() {

		// clear screen
		for (int i = 0; i < RENDER_WIDTH * RENDER_HEIGHT; ++i) {
			screen[i].Char.AsciiChar = ' ';
			screen[i].Attributes = 0;
		}

		// render every col on the screen to make walls
		for (int x = 0; x < RENDER_WIDTH; ++x) {

			float rayAngle = (playerAngle - fov / 2.0f) + ((float)x / (float)RENDER_WIDTH) * fov;

			// casting ray to return wall distance
			float distance = castRay(rayAngle);

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

				if (y < wallTop) {												// upper the wall
					screen[screenIndex].Char.AsciiChar = RenderObjects::PATH;
					//screen[screenIndex].Attributes = BACKGROUND_BLUE >> 1; // set color
				}
				else if (y >= wallTop && y <= wallBottom) {						// in-wall
					screen[screenIndex].Char.AsciiChar = getWallSymb(distance);
					//screen[screenIndex].Attributes = getWallColor(distance);
				}
				else {															// under the wall
					screen[screenIndex].Char.AsciiChar = RenderObjects::PATH;
					//screen[screenIndex].Attributes = BACKGROUND_GREEN >> 1;
				}
			}

		}

		// another renderable things here

		renderPlayerInfo();
		renderMiniMap();
		

		// draw final buffer to the screen (render -> screen)
		SMALL_RECT writeRegion = { 0, 0, RENDER_WIDTH - 1, RENDER_HEIGHT - 1 };
		WriteConsoleOutput(hConsole, screen, { RENDER_WIDTH, RENDER_HEIGHT }, { 0, 0 }, &writeRegion);
	}

	void renderPlayerInfo() {

		char infoBuf[64];

		sprintf_s(infoBuf, "X:%.2f Y:%0.2f Angle:%0.2f", playerCoord.x, playerCoord.y, playerAngle);

		for (int i = 0; i < strlen(infoBuf) && i < RENDER_WIDTH; ++i) {
			screen[i].Char.AsciiChar = infoBuf[i];
			screen[i].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
	}

	void renderMiniMap() {
		int miniMapSize = 16;	// but would be better to scale of real map sizes

		coord_t offset = { RENDER_WIDTH - miniMapSize - 1, 1 };

		for (int y = 0; y < MAP_HEIGHT && y < miniMapSize; y++)
			for (int x = 0; x < MAP_WIDTH && x < miniMapSize; x++) {
				int screenX = offset.x + x;
				int screenY = offset.y + y;

				int screenIndex = screenY * RENDER_WIDTH + screenX;	// scale with render width

				if (screenX >= 0 && screenX < RENDER_WIDTH && screenY >= 0 && screenY < RENDER_HEIGHT) {

					screen[screenIndex].Char.AsciiChar = map[y][x];

					if (map[y][x] == '#') 
						screen[screenIndex].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					else 
						screen[screenIndex].Attributes = FOREGROUND_BLUE;
					

					// player pos
					if ((int)playerCoord.x == x && (int)playerCoord.y == y) {
						screen[screenIndex].Char.AsciiChar = '@';
						screen[screenIndex].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
					}
				}
			}
		
	}

	// render (game right now) runing
	void run() {
		bool isRunning = true;

		while (isRunning) {
			
			handleInput();
			
			render();

			// stop render when Esc pressed
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) isRunning = false;

			Sleep(16); // 16 ms ~60 FPS, delay to stabilize frames
		}

	}


	// class utils
private:

	// normalize angle to [0; 2pi]
	void normalizeAngle(float& angle) {
		while (angle < 0) angle += 2 * rendPI;
		while (angle >= 2 * rendPI) angle -= 2 * rendPI;
	}


	std::vector<std::vector<char>> generateMap() {

		std::vector<std::vector<char>> newMap(MAP_HEIGHT, std::vector<char>(MAP_WIDTH));

		//char** newMap = new char* [MAP_HEIGHT];

		for (int i = 0; i < MAP_HEIGHT; ++i) {

			//newMap[i] = new char[MAP_WIDTH];

			for (int j = 0; j < MAP_WIDTH; ++j) {

				if (!i || !j || j == MAP_WIDTH - 1 || i == MAP_HEIGHT - 1) {

					//newMap[i].push_back(RenderObjects::WALL);
					
					// coz dimentions are presized now
					newMap[i][j] = RenderObjects::WALL;
				}
				else {
					newMap[i][j] = RenderObjects::PATH;
					//newMap[i].push_back(RenderObjects::PATH);
				}
			}
		}

		coord_t coords = { 2, 2 };	// border is already made, and second cell used for player spawn right now

		// randomly spawn walls
		for (int i = coords.x; i < MAP_HEIGHT - 2; ++i)
			for (int j = coords.y; j < MAP_WIDTH - 2; ++j) {
				coord_t spawnCoords = { i , j };

				//if (RenderObjects::isWallSpawnable(spawnCoords, MAP_HEIGHT, MAP_WIDTH))
				if (RenderObjects::Utils::isWallPercentSpawnable(spawnCoords, 30))		// 40% by default
					newMap[i][j] = RenderObjects::WALL;
			}

		newMap[coords.x][coords.y] = RenderObjects::PATH;

		return newMap;
	}


	void handleInput() {

		float movementSpeed = 0.1f;
		float rotationSpeed = 0.1f;

		// rotate to A-D
		// move forward-backward to W-S
		if (GetAsyncKeyState('A') & 0x8000)
			playerAngle -= rotationSpeed;
		else if (GetAsyncKeyState('D') & 0x8000)
			playerAngle += rotationSpeed;
		else {

			fcoord_t newPlayerCoord = playerCoord;

			if (GetAsyncKeyState('W') & 0x8000) {

				newPlayerCoord.x = playerCoord.x + cosf(playerAngle) * movementSpeed;
				newPlayerCoord.y = playerCoord.y + sinf(playerAngle) * movementSpeed;
			}
			else if (GetAsyncKeyState('S') & 0x8000) {

				newPlayerCoord.x = playerCoord.x - cosf(playerAngle) * movementSpeed;
				newPlayerCoord.y = playerCoord.y - sinf(playerAngle) * movementSpeed;
			}

			if (map[(int)newPlayerCoord.y][(int)newPlayerCoord.x] == RenderObjects::PATH)
				playerCoord = newPlayerCoord;

			normalizeAngle(playerAngle);
		}
	}
};