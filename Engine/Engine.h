#pragma once

#include "common_utils.h"
#include "spawn_utils.h"
#include "Renderer.h"

class Engine {

	// in-game data
	// player data
	fcoord_t playerCoord; // x, y
	float playerAngle;

	// array to store game map
	std::vector<std::vector<char>> map;			// MAP_HEIGHT x MAP_WIDTH

	static constexpr float MOVEMENT_SPEED = 0.25f;
	static constexpr float ROTATION_SPEED = 0.3f;

	// game map array size
	static constexpr int MAP_HEIGHT = 16;
	static constexpr int MAP_WIDTH = 16;

	Renderer renderer;	// Renderer class object: renderer.render()

	bool isRunning;

public:

	Engine() : playerCoord({ 2.0f, 2.0f }), playerAngle(0.0f), isRunning(true) {

		srand(static_cast<unsigned int>(time(NULL)));
		map = generateMap();
	}

	// run game-cycle
	void run();

	// show created/choosen map (for debug, not for player)
	void showMap() const {

		for (const auto& row : map) {
			for (const auto cell : row)
				std::cout << cell;
			std::cout << std::endl;
		}
	}

// getters for Renderer:
	const std::vector<std::vector<char>>& getMap() const { return map; }
	const fcoord_t& getPlayerCoord() const { return playerCoord; }
	float getPlayerAngle() const { return playerAngle; }

private:
	
	// Generate in-scene map randomly
	std::vector<std::vector<char>> generateMap();

	// User input processing
	void handleInput();

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);

};