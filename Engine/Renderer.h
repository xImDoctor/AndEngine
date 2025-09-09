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
	
	// use string as simple console buffer
	std::vector<std::string> screenBuffer;

public:

	Renderer();
	~Renderer() = default;

	// render running, game logic runnin in the Engine now
	//void run();

	// Main rendering function with all renderable elements implemented
	// runs rendering now
	void render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle);	

	// info rendering
	void renderPlayerInfo(const fcoord_t& playerCoord, float playerAngle);
	void renderMiniMap(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord);

// render delay getter
	inline int getRenderDelay() const{
		return RENDER_DELAY;
	}

private:

	// simple raycast algorithm, returns float distance from player to object
	float castRay(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle);

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
};