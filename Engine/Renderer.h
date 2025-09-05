#pragma once

#include <Windows.h>

#include <vector>
#include <string>
#include <algorithm> // fill

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cctype>	// ctype.h

#include <cstdio>
#include <iostream>



namespace RenderConst {

	// simplified (shorten) Pi pre-defined, not const from math.h
	// also other values with Pi radian
	namespace Math {
		static constexpr float PI = 3.1415926535f;		// Pi
		static constexpr float TWO_PI = PI * 2.0f;		// 2Pi
		static constexpr float HALF_PI = PI * 0.5f;		// Pi/2
		static constexpr float QUART_PI = PI * 0.25f;	// Pi/4
	}
}

// also:
// todo: move map creation to Game class (but right now it is simplified)
// todo: choose drawing color for wall based on its disntace - getWallColor
// HAVE TO DO A LOT OF REFACTORING!


struct coord_t {
	int x, y;

	// overload for == operator to use coord_t directly in the conditions
	friend bool operator==(const coord_t& coords1, const coord_t& coords2) {
		return coords1.x == coords2.x && coords1.y == coords2.y;
	}

	friend bool operator!=(const coord_t& coords1, const coord_t& coords2) { 
		return !(coords1 == coords2);
	}
};


struct fcoord_t {
	float x, y;

	friend bool operator==(const fcoord_t& coords1, const fcoord_t& coords2) {
		return coords1.x == coords2.x && coords1.y == coords2.y;
	}

	friend bool operator!=(const fcoord_t& coords1, const fcoord_t& coords2) {
		return !(coords1 == coords2);
	}
};


namespace Render {

	namespace Utils {

		void MoveToXY(int x, int y) {

			COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };

			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(output, pos);
		}

		void MoveToXY(const coord_t& coords) {

			COORD pos = { static_cast<SHORT>(coords.x), static_cast<SHORT>(coords.y) };

			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(output, pos);
		}

		void SetTextColor(int color) {

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, color);
		}

		// set default color inline function instead of pre-defined macro
		inline void setDefaultColor() {
			Utils::SetTextColor(Objects::Colors::Default);
		}
	}

	namespace Objects {

		enum objectTypes { PATH = ' ', WALL = '#' };


		// to make material and other params for objects (empty for now)
		struct renderer_object_t {

		};

		// colors of objects
		namespace Colors {

			// constexpr color definition instead of magical values in the define or function
			constexpr WORD Default = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;			// white
			constexpr WORD BrightWhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			constexpr WORD Yellow = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			constexpr WORD Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			constexpr WORD BrightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			constexpr WORD RedGreen = FOREGROUND_RED | FOREGROUND_GREEN;
			constexpr WORD RedBlue = FOREGROUND_RED | FOREGROUND_BLUE;
			constexpr WORD Blue = FOREGROUND_BLUE;
			constexpr WORD BrightRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
	}

}

namespace SpawnUtils {

	// spawn wall if taken coords == random coords width got from area
	// use srand() on the game initialization, this is not the task of Renderer
	// todo: move map creation to Game class (but right now it is simplified)
	bool isWallSpawnable(const coord_t& coords, const int areaHeight, const int areaWidth) {

		coord_t spawnCoords = { rand() % areaHeight, rand() % areaWidth };

		return coords == spawnCoords;
	}

	bool isWallPercentSpawnable(const coord_t& coords, int spawnPercent = 40) {
		return (rand() % 100 + 1) <= spawnPercent;
	}
}

// try to make base raytracing system - Console Raycaster
class Renderer {

	// render window size
	static constexpr int RENDER_HEIGHT = 60;
	static constexpr int RENDER_WIDTH = 280;

	// array size
	static constexpr int MAP_HEIGHT = 16;
	static constexpr int MAP_WIDTH = 16;

	static constexpr int MINIMAP_SIZE = MAP_HEIGHT;

	static constexpr int RENDER_DELAY = 50;		// ms, reduced to increase FPS

	// array to store game map
	std::vector<std::vector<char>> map;			// MAP_HEIGHT x MAP_WIDTH

	static constexpr float FOV = RenderConst::Math::QUART_PI;	// field of view, pi/4
	static constexpr float depth = 16.0f;						// max render distance

	// player data
	fcoord_t playerCoord; // x, y
	float playerAngle;		

	static constexpr float MOVEMENT_SPEED = 0.25f;
	static constexpr float ROTATION_SPEED = 0.3f;


	// use string as simple console buffer
	std::vector<std::string> screenBuffer;

public:

	Renderer() {

		srand(static_cast<unsigned int>(time(NULL)));

		playerCoord = { 2.0f, 2.0f };
		playerAngle = 0.0f;

		screenBuffer.resize(RENDER_HEIGHT);	// vector size, not string one
		for (auto& line : screenBuffer)
			line.resize(RENDER_WIDTH, ' '); // resize and clear strings

		
		// hide cursor when console is active
		CONSOLE_CURSOR_INFO cursorInfo;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		map = generateMap();

	}

	~Renderer() = default;

	// show created/choosen map (for debug, not for player)
	void showMap() const {

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
				if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
					isWallHit = true;
			}

		}

		return distance;
	}


	// Get drawing symbol for wall based on its distance
	char getObjectSymb(float distance) {

		static const char symbs[] = { ' ', '#', 'X', 'O', 'x', ' - ', '.'};				// nothing as 0
		static constexpr int symbArrSize = (int)(sizeof(symbs) / (sizeof(symbs[0])));	// to know it when compiled
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

	// Chooses symbol color depending on its shape and sets color to it
	void setObjectColor(const char symb) {

		switch (symb) {
			// in-screen walls displaying colors
		case '#':
			Render::Utils::SetTextColor(Render::Objects::Colors::BrightWhite);
			break;
		case 'X':
			if (!y)	// if this is X axis in the info label
				Render::Utils::SetTextColor(Render::Objects::Colors::BrightGreen);
			else // 'X' as wall symbol
				Render::Utils::SetTextColor(Render::Objects::Colors::Default); // white
			break;
		case 'O':
			Render::Utils::SetTextColor(Render::Objects::Colors::RedGreen);
			break;
		case 'x':
			Render::Utils::SetTextColor(Render::Objects::Colors::RedBlue);
			break;
		case '-':
			Render::Utils::SetTextColor(Render::Objects::Colors::Blue);
			break;
		case '.':
			Render::Utils::setDefaultColor();
			break;
		case '@':
			Render::Utils::SetTextColor(Render::Objects::Colors::BrightRed);
			break;
		default:

			if (isalpha(symb) || isdigit(symb))		// zero-line player info, recolor only text/nums
				Render::Utils::SetTextColor(Render::Objects::Colors::BrightGreen);
			else Render::Utils::setDefaultColor();
			break;
		}

	}


	void render() {

		// removed cls
		// system("cls");

		// render every col on the screen to make walls
		for (int x = 0; x < RENDER_WIDTH; ++x) {

			float rayAngle = (playerAngle - FOV / 2.0f) + ((float)x / (float)RENDER_WIDTH) * FOV;

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

				if (y < wallTop) 											// upper the wall
					screenBuffer[y][x] = Render::Objects::PATH;
				else if (y >= wallTop && y <= wallBottom)					// in-wall
					screenBuffer[y][x] = getObjectSymb(distance);
				else 														// under the wall
					screenBuffer[y][x] = Render::Objects::PATH;
			}

		}

		// another renderable things here
		renderPlayerInfo();
		renderMiniMap();

		// draw final buffer to the screen (render -> screen)
		displayScreen();
	}

	void renderPlayerInfo() {

		char infoBuf[64];
		sprintf_s(infoBuf, "X:%.2f Y:%0.2f Angle:%0.2f", playerCoord.x, playerCoord.y, playerAngle);	// setup buffer

		std::string info = infoBuf;
		if (info.length() > RENDER_WIDTH) info.resize(RENDER_WIDTH);

		for (int i = 0; i < strlen(infoBuf) && i < RENDER_WIDTH; ++i)
			screenBuffer[0][i] = info[i];
	}

	void renderMiniMap() {
		int miniMapSize = MINIMAP_SIZE;	// scaled one now

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
		for (int y = 0; y < MAP_HEIGHT && y < miniMapSize; ++y)
			for (int x = 0; x < MAP_WIDTH && x < miniMapSize; ++x) {

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

	// render (game right now) runing
	void run() {
		bool isRunning = true;

		system("cls");			// clear window before game drawing
		while (isRunning) {
			
			handleInput();
			
			render();

			// stop render when Esc pressed
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) isRunning = false;

			// sleep time with delay to stabilize frames (~FPS)
			Sleep(RENDER_DELAY);
		}

	}


// class utils
private:

	// normalize angle to [0; 2pi]
	void normalizeAngle(float& angle) {
		while (angle < 0) angle += RenderConst::Math::TWO_PI;
		while (angle >= RenderConst::Math::TWO_PI) angle -= RenderConst::Math::TWO_PI;
	}

	// Clears screenBuffer
	void clearScreenBuffer() {

		for (auto& line : screenBuffer)
			std::fill(line.begin(), line.end(), ' ');
	}


	// display buffer through console output and also define elements colors
	void displayScreen() {

		//clearScreenBuffer();

		Render::Utils::MoveToXY(0, 0);
		for (int y = 0; y < RENDER_HEIGHT; ++y) {
			Render::Utils::MoveToXY(0, y);		// move to start of every Y line

			for (int x = 0; x < RENDER_WIDTH; ++x) {
				
				char symb = screenBuffer[y][x];
				setObjectColor(symb);

				// display symbol after coloring
				std::cout << symb;
			}
		}

		Render::Utils::setDefaultColor();
		std::cout.flush(); // removed cls so flush out buffer directly
	}


	std::vector<std::vector<char>> generateMap() {

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


	void handleInput() {

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
};