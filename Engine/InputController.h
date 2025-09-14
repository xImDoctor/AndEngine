#pragma once

#include <Windows.h>
#include <vector>

#include "utils/common_utils.h"
#include "utils/math_utils.h"

#include <unordered_map>
#include <array>


class InputController {

	static constexpr size_t MAX_KEYS = 256; // for all ASCII identification
	static constexpr int TRACKED_KEY_COUNT = 5;

	std::array<int, TRACKED_KEY_COUNT> TRACKED_KEYS = { 'W', 'A', 'S', 'D', VK_ESCAPE };

	std::array<bool, MAX_KEYS> keyStates{};
	std::array<bool, MAX_KEYS> prevKeyStates{};

	//std::unordered_map<int, bool> keyStateMap;		// to store key state
	//std::unordered_map<int, bool> prevKeyStateMap;	// to store and check if key pressed in next frame

public:


	InputController() = default;

	void update() {

		for (int key : TRACKED_KEYS) {
			prevKeyStates[key] = keyStates[key];
			keyStates[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
		}
	}

	// not pressed at prevState and pressed in current one
	bool isKeyPressed(int key) const {
		return key < MAX_KEYS ? !prevKeyStates[key] && keyStates[key] : false;
	}

	// just track if current one not pressed
	bool isKeyDown(int key) const {
		return key < MAX_KEYS ? keyStates[key] : false;
	}


	// holding key: prevstate and current state are the same and true
	bool isKeyHeld(int key) const {
		return key < MAX_KEYS ? prevKeyStates[key] && keyStates[key] : false;
	}

	// after holding: was pressed but now released
	bool isKeyReleased(int key) const {
		return key < MAX_KEYS ? prevKeyStates[key] && !keyStates[key] : false;
	}

	// rotate to A-D
	// move forward-backward to W-S
	void processInput(const std::vector<std::vector<char>>& map, char pathSymb, fcoord_t& playerCoord, float& playerAngle, float moveSpeed, float rotationSpeed) {

#if _DEBUG
		printf("Player: (%.1f,%.1f), Map: %dx%d\n",
			playerCoord.x, playerCoord.y,
			(int)map[0].size(), (int)map.size());
#endif


		// no map - nothing to process
		if (map.empty() || map[0].empty())
			return;

		int mapHeight = static_cast<int>(map.size());
		int mapWidth = static_cast<int>(map[0].size());


		bool rotated = false;
		if (isKeyDown('A')) {
			playerAngle -= rotationSpeed;
			rotated = true;
		}
		else if (isKeyDown('D')) {
			playerAngle += rotationSpeed;
			rotated = true;
		}

		if (rotated) {
			MathUtils::Angles::normalizeAngle(playerAngle);
			return;
		}
		
		bool moved = false;
		fcoord_t newPlayerCoord = playerCoord;
		if (isKeyDown('W')) {

			newPlayerCoord.x = playerCoord.x + cosf(playerAngle) * moveSpeed;
			newPlayerCoord.y = playerCoord.y + sinf(playerAngle) * moveSpeed;
			
			moved = true;
		}
		else if (isKeyDown('S')) {

			newPlayerCoord.x = playerCoord.x - cosf(playerAngle) * moveSpeed;
			newPlayerCoord.y = playerCoord.y - sinf(playerAngle) * moveSpeed;

			moved = true;
		}

		if (moved && isValidPosition(newPlayerCoord, mapWidth, mapHeight, map, pathSymb))
			playerCoord = newPlayerCoord;
	}

	inline bool isEscPressed() {
		return isKeyPressed(VK_ESCAPE);
	}

private:
	bool isValidPosition(const fcoord_t& pos, int mapWidth, int mapHeight, const std::vector<std::vector<char>>& map, char pathSymb) const {

		// float to int
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
			return false;

		return map[y][x] == pathSymb;
	}

};