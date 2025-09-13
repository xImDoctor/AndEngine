#pragma once


namespace MathUtils {

	namespace Consts {
		// simplified (shorten) Pi pre-defined, not const from math.h
		// also other values with Pi radian
		static constexpr float PI = 3.1415926535f;		// Pi
		static constexpr float TWO_PI = PI * 2.0f;		// 2Pi
		static constexpr float HALF_PI = PI * 0.5f;		// Pi/2
		static constexpr float QUART_PI = PI * 0.25f;	// Pi/4
		
	}


	namespace Angles {

		// normalize angle to [0; 2pi] (used by Engine/Renderer)
		constexpr static void normalizeAngle(float& angle) noexcept {
			while (angle < 0) angle += Consts::TWO_PI;
			while (angle >= Consts::TWO_PI) angle -= Consts::TWO_PI;
		}

	}

	namespace Func {
		inline int _min(int a, int b) {
			return a < b ? a : b;
		}

		inline int _max(int a, int b) {
			return a < b ? b : a;
		}
	}

}