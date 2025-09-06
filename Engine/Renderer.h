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

#include "render_utils.h"	// namespace RenderConsts, Render (Utils, Objects::Colors)
#include "spawn_utils.h"	// isWallSpawnable


// todo: move map creation to Game class (but right now it is simplified)


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

	Renderer();
	~Renderer() = default;

	// render (game right now) runing
	void run();

	// show created/choosen map (for debug, not for player)
	void showMap() const {

		for (const auto& row : map) {
			for (const auto cell : row)
				std::cout << cell;
			
			std::cout << std::endl;
		}
	}

	// simple raycast algorithm, returns float distance from player to object
	float castRay(float rayAngle);

	// Get drawing symbol for wall based on its distance
	char getObjectSymb(float distance);

	// Chooses symbol color depending on its shape and sets color to it
	// predefine with 1 not to use when not written in other code
	void setObjectColor(const char symb, const int heightValue = 1);

	// Main rendering function with all renderable elements implemented
	void render();

	// info rendering
	void renderPlayerInfo();
	void renderMiniMap();

// class utils
private:

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);

	// Clears screenBuffer
	void clearScreenBuffer();

	// Display buffer through console output and also define elements colors
	void displayScreen();

	// Generate in-scene map randomly
	std::vector<std::vector<char>> generateMap();

	// User input processing
	void handleInput();

};