#pragma once

#include <Windows.h>
#include "common_utils.h"


namespace Render {

	namespace Objects {

		enum objectTypes { PATH = ' ', WALL = '#' };


		// to make material and other params for objects (empty for now)
		struct renderer_object_t {

		};

		// colors of objects
		namespace Colors {

			// constexpr color definition instead of magical values in the define or function
			// WORD = unsigned short in Win type definition
			constexpr WORD White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;			// white
			constexpr WORD BrightWhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			constexpr WORD Yellow = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			constexpr WORD Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			constexpr WORD BrightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			constexpr WORD RedGreen = FOREGROUND_RED | FOREGROUND_GREEN;
			constexpr WORD RedBlue = FOREGROUND_RED | FOREGROUND_BLUE;
			constexpr WORD Blue = FOREGROUND_BLUE;
			constexpr WORD BrightRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
	}

	namespace Utils {

		void MoveToXY(int x, int y);
		void MoveToXY(const coord_t& coords);

		void SetTextColor(WORD color);

		namespace objColors = ::Render::Objects::Colors;
		// set default color inline function instead of pre-defined macro
		inline void setDefaultColor() {
			SetTextColor(objColors::White);
		}
	}
}