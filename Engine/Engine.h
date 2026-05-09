#pragma once

#include "utils/common_utils.h"
#include "utils/math_utils.h"

#include <cstdlib>
#include <ctime>

#include <chrono>

#include "InputController.h"
#include "MapGenerator.h"

// Renderers
#include "gl/GLRenderer.h"
#include "ConsoleRenderer.h"


class Engine {

	// in-game data
	// player data
	fcoord_t playerCoord; // x, y
	float playerAngle;

	// before frameTime implemention
	//static constexpr float MOVEMENT_SPEED = 0.25f;
	//static constexpr float ROTATION_SPEED = 0.25f;

	// after frameTime
	static constexpr float MOVEMENT_SPEED = 5.0f;
	static constexpr float ROTATION_SPEED = 5.0f;

	MapGenerator generator;		// game map data moved to MapGenerator
	InputController controller;	// processes player input, key press and changes player coord, angle (processing map movement)

	// Now 2 renderers: console, gl
	std::unique_ptr<IRenderer> renderer;

	bool isRunning;

	float deltaTime; // time between frames in sec, to smooth Engine system calling
	static constexpr float MAX_DELTA_TIME = 1.0f / 30.0f;		// lock max fps with 30
	static constexpr float TARGET_FPS = 60.0f;
	static constexpr float FRAME_TIME = 1.0f / TARGET_FPS;

public:

	Engine() : playerCoord({ 2.0f, 2.0f }), playerAngle(0.0f), isRunning(true), deltaTime(0.0f)
	{
		// trig tables precompute (optimization) now in RaycastEngine
	}

	// run game-cycle
	// with flag to switch between sizeStepped and DDA renders
	void run(bool useOpenGLRenderer = false, bool use_DDA_render = false);


// getters for Renderer:

	inline const fcoord_t& getPlayerCoord() const { return playerCoord; }
	inline float getPlayerAngle() const { return playerAngle; }

	inline static float getRotationSpeed() { return ROTATION_SPEED; }
	inline static float getMovementSpeed() { return MOVEMENT_SPEED; }

// with map generator now
	inline int getMapWidth() const { return generator.getMapWidth(); }
	inline int getMapHeight() const { return generator.getMapHeight(); }
	inline int getMapSeed() const { return generator.getMapSeed(); }
	inline std::vector<std::vector<char>> getMap() const { return generator.getMap(); }

// maybe unsafe but get generator to use its methods directly
	inline MapGenerator& useMapGenerator() {
		return generator;
	}

	//inline void showPrecomputedTrigVals() {
	//	renderer.showPrecomputedTrigTables();
	//}


private:
	
	// User input processing
	//void handleInput();		// InputController now

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);

};