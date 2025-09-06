#pragma once

#include <Windows.h>
#include "eng_structs.h"	// coord_t, fcoord_t


namespace RenderConst {

	// simplified (shorten) Pi pre-defined, not const from math.h
	// also other values with Pi radian
	namespace Math {
		static constexpr float PI = 3.1415926535f;		// Pi
		static constexpr float TWO_PI = PI * 2.0f;		// 2Pi
		static constexpr float HALF_PI = PI * 0.5f;		// Pi/2
		static constexpr float QUART_PI = PI * 0.25f;	// Pi/4
	}
}


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

		void MoveToXY(int x, int y) {

			COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };

			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(output, pos);
		}

		void MoveToXY(const coord_t& coords) {

			COORD pos = { static_cast<SHORT>(coords.x), static_cast<SHORT>(coords.y) };

			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(output, pos);
		}

		void SetTextColor(int color) {

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, color);
		}

		namespace objColors = ::Render::Objects::Colors;

		// set default color inline function instead of pre-defined macro
		inline void setDefaultColor() {
			SetTextColor(objColors::White);
		}
	}
}