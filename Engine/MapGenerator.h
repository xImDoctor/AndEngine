#pragma once

#include <iostream>

#include <vector>

#include <cstdlib>
#include <ctime>

#include "Renderer.h"
#include "utils/spawn_utils.h"

class MapGenerator {

	static constexpr int MAP_HEIGHT = 16;
	static constexpr int MAP_WIDTH = 16;

	unsigned int mapSeed;
	std::vector<std::vector<char>> map;

public:
	MapGenerator() : mapSeed(static_cast<unsigned int>(time(NULL))) {

		regenerateMap();	// sets seed to srand and generates map using it
	}

// getters
	inline const std::vector<std::vector<char>>& getMap() const { return map; }
	inline unsigned int getMapSeed() const { return mapSeed; }

	inline int getMapWidth() const { return MAP_WIDTH; }
	inline int getMapHeight() const { return MAP_HEIGHT; }

// setter
	void changeMapSeed(unsigned int newSeed) {
		mapSeed = newSeed;
		regenerateMap();
	}


	// show created/choosen map (for debug, not for player)
	void showMap() const {

		for (const auto& row : map) {
			for (const auto cell : row)
				std::cout << cell;
			std::cout << std::endl;
		}
	}

private:

	// Generate in-scene map randomly
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

	// Regenerate map with choosen seed
	inline void regenerateMap() {
		srand(mapSeed);
		map = generateMap();
	}

};