#pragma once

#include "utils/common_utils.h"
#include "utils/math_utils.h"

#include <cstdlib>
#include <ctime>

#include "InputController.h"
#include "MapGenerator.h"
#include "Renderer.h"

class Engine {

	// in-game data
	// player data
	fcoord_t playerCoord; // x, y
	float playerAngle;

	static constexpr float MOVEMENT_SPEED = 0.25f;
	static constexpr float ROTATION_SPEED = 0.25f;

	MapGenerator generator;		// game map data moved to MapGenerator
	InputController controller;	// processes player input, key press and changes player coord, angle (processing map movement)
	Renderer renderer;			// Renderer class object: renderer.render(useDDA = false)

	bool isRunning;

public:

	Engine() : playerCoord({ 2.0f, 2.0f }), playerAngle(0.0f), isRunning(true){	}

	// run game-cycle
	// with flag to switch between sizeStepped and DDA renders
	void run(bool use_DDA_render = false);


// getters for Renderer:

	inline const fcoord_t& getPlayerCoord() const { return playerCoord; }
	inline float getPlayerAngle() const { return playerAngle; }

// with map generator now
	inline int getMapWidth() const { return generator.getMapWidth(); }
	inline int getMapHeight() const { return generator.getMapHeight(); }
	inline int getMapSeed() const { return generator.getMapSeed(); }
	inline std::vector<std::vector<char>> getMap() const { return generator.getMap(); }

// maybe unsafe but get generator to use its methods directly
	inline MapGenerator& useMapGenerator() {
		return generator;
	}


private:
	
	// User input processing
	//void handleInput();		// InputController now

	// Normalize angle to [0; 2pi]
	void normalizeAngle(float& angle);

};