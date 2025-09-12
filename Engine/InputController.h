#pragma once

#include <Windows.h>
#include <vector>

#include "utils/common_utils.h"
#include "utils/math_utils.h"


static class InputController {

public:

	InputController() = default;

	void processInput(const std::vector<std::vector<char>>& map, char pathSymb, fcoord_t& playerCoord, float& playerAngle, float moveSpeed, float rotationSpeed) {


		// rotate to A-D
		// move forward-backward to W-S
		if (GetAsyncKeyState('A') & 0x8000)
			playerAngle -= rotationSpeed;
		else if (GetAsyncKeyState('D') & 0x8000)
			playerAngle += rotationSpeed;
		else {

			fcoord_t newPlayerCoord = playerCoord;

			if (GetAsyncKeyState('W') & 0x8000) {

				newPlayerCoord.x = playerCoord.x + cosf(playerAngle) * moveSpeed;
				newPlayerCoord.y = playerCoord.y + sinf(playerAngle) * moveSpeed;
			}
			else if (GetAsyncKeyState('S') & 0x8000) {

				newPlayerCoord.x = playerCoord.x - cosf(playerAngle) * moveSpeed;
				newPlayerCoord.y = playerCoord.y - sinf(playerAngle) * moveSpeed;
			}

			if (map[(int)newPlayerCoord.y][(int)newPlayerCoord.x] == pathSymb)
				playerCoord = newPlayerCoord;

			MathUtils::Angles::normalizeAngle(playerAngle);
		}
	}

	inline bool isEscPressed() {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			return true;

		return false;
	}
};