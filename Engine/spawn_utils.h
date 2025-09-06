#pragma once

#include <cstdlib>
#include "eng_structs.h"	// coord_t, fcoord_t

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