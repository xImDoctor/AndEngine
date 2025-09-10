#pragma once

#include "utils/common_utils.h"
#include "utils/spawn_utils.h"
#include "utils/math_utils.h"

#include "Renderer.h"

class Engine {

	// in-game data
	// player data
	fcoord_t playerCoord; // x, y
	float playerAngle;

	// array to store game map
	std::vector<std::vector<char>> map;			// MAP_HEIGHT x MAP_WIDTH
	unsigned int mapSeed;

	static constexpr float MOVEMENT_SPEED = 0.25f;
	static constexpr float ROTATION_SPEED = 0.25f;

	// game map array size
	static constexpr int MAP_HEIGHT = 16;
	static constexpr int MAP_WIDTH = 16;

	Renderer renderer;	// Renderer class object: renderer.render(useDDA = false)

	bool isRunning;

public:

	Engine() : playerCoord({ 2.0f, 2.0f }), playerAngle(0.0f), isRunning(true),
	mapSeed(static_cast<unsigned int>(time(NULL))) {

		regenerateMap();
		//srand(mapSeed);
		//map = generateMap();
	}

	// run game-cycle
	// with flag to switch between sizeStepped and DDA renders
	void run(bool use_DDA_render = false);

	// show created/choosen map (for debug, not for player)
	void showMap() const {

		for (const auto& row : map) {
			for (const auto cell : row)
				std::cout << cell;
			std::cout << std::endl;
		}
	}

// getters for Renderer:
	inline const std::vector<std::vector<char>>& getMap() const { return map; }
	inline const fcoord_t& getPlayerCoord() const { return playerCoord; }
	inline float getPlayerAngle() const { return playerAngle; }
	inline unsigned int getMapSeed() const { return mapSeed; }

// setters

	// use your own seed another from random one, also regenerates map with new random
	void changeMapSeed(unsigned int newSeed) {
		mapSeed = newSeed;
		regenerateMap();
	}


private:
	
	// Generate in-scene map randomly
	std::vector<std::vector<char>> generateMap();

	// User input processing
	void handleInput();

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);

	// Regenerate map with choosen seed
	void regenerateMap() {
		srand(mapSeed);
		map = generateMap();
	}

};