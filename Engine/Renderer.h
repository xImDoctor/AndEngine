#pragma once

#include <Windows.h>

#include <vector>
#include <string>
#include <algorithm>	// fill

#include <limits>		// numeric_limits

#include <cmath>
#include <cctype>		// ctype.h

#include <cstdio>
#include <iostream>

#include "utils/common_utils.h"
#include "utils/render_utils.h"	// namespace RenderConsts, Render (Utils, Objects::Colors)
#include "utils/math_utils.h"

// Base raycasting system - Console Raycaster
class Renderer {

	// render window size
	static constexpr int RENDER_HEIGHT = 60;
	static constexpr int RENDER_WIDTH = 280;

	static constexpr int MINIMAP_SIZE = 16;

	static constexpr int RENDER_DELAY = 50;		// ms, reduced to increase FPS

	static constexpr float FOV = MathUtils::Consts::QUART_PI;	// field of view, pi/4
	static constexpr float depth = 16.0f;						// max render distance

	// cache cos/sin instead of each step calculating
	std::vector<float> sinTable, cosTable;
	float angleStep;
	static constexpr int TRIG_TABLE_SIZE = 3600;  // 0.1 precision points
	
	// use string as simple console buffer
	std::vector<std::string> screenBuffer;

	// console handler moved here
	HANDLE hConsole;

public:

	Renderer();
	~Renderer() = default;

	// render running, game logic runnin in the Engine now
	//void run();

	// Main rendering function with all renderable elements implemented
	// Flag useDDA = true enables DDA raycast algorithm else function uses stepped one.
	// Stepped one choosen by default (useDDA = false)
	void render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle, bool useDDA = false);	

	// info rendering
	void renderPlayerInfo(const fcoord_t& playerCoord, float playerAngle);
	void renderMiniMap(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord);

// render delay getter
	inline int getRenderDelay() const{
		return RENDER_DELAY;
	}

private:

	// simple raycast algorithm, returns float distance from player to object
	float castRay_stepped(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle);

	// Implementation of faster DDA raycast algorithm (improve x10)
	// Optimized algorithm that crosses grid from one cell to second with one step only.
	// Guaranteed to visit every cell in the ray path once
	float castRay_dda(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle);


	// Get drawing symbol for wall based on its distance
	char getObjectSymb(float distance);

	// Chooses symbol color depending on its shape and sets color to it
	// predefine with 1 not to use when not written in other code
	void setObjectColor(const char symb, const int heightValue = 1);


// class utils:

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);	// uses math_utils.h now

	// Clears screenBuffer
	void clearScreenBuffer();

	// Display buffer through console output and also define elements colors
	void displayScreen();


	// fast search of cached cos
	inline float getCachedCos(float angle) const {

		// normalization
		MathUtils::Angles::normalizeAngle(angle);

		// find index in the table
		int index = static_cast<int>(angle / angleStep);
		if (index >= cosTable.size()) 
			index = 0;

		return cosTable[index];
	}

	// sin analog
	inline float getCachedSin(float angle) const {

		MathUtils::Angles::normalizeAngle(angle);

		int index = static_cast<int>(angle / angleStep);
		if (index >= sinTable.size()) index = 0;

		return sinTable[index];
	}


public:
// pre-compute:
// 
	// Computes sin, cos values via compilation, not to compute it every raycasting step
	void precomputeTrigTables();
	// Show computed values
	void showPrecomputedTrigTables();
};