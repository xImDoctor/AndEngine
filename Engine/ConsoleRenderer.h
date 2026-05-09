#pragma once

#include <Windows.h>

#include <string>

#include <cctype>		// ctype.h
#include <cstdio>

#include "RaycastEngine/RaycastEngine.h" 

#include "IRenderer.h"


// Console Renderer (uses RaycastEngine now)
class ConsoleRenderer : public IRenderer {

	// render window size
	static constexpr int RENDER_HEIGHT = 60;
	static constexpr int RENDER_WIDTH = 280;

	static constexpr int MINIMAP_SIZE = 16;					

	// use string as simple console buffer
	std::vector<std::string> screenBuffer;

	// console handler moved here
	HANDLE hConsole;

	// Raycast engine obj to call castRAy
	RaycastEngine raycast;

public:

	ConsoleRenderer();
	~ConsoleRenderer() = default;

	// Main rendering function with all renderable elements implemented
	// Flag useDDA = true enables DDA raycast algorithm else function uses stepped one.
	// Stepped one choosen by default (useDDA = false)
	void render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle, bool useDDA = false);	

	// info rendering
	void renderPlayerInfo_buffered(const fcoord_t& playerCoord, float playerAngle);
	void renderMiniMap(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord);

private:

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