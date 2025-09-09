#include "render_utils.h"


void Render::Utils::MoveToXY(int x, int y) {

	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void Render::Utils::MoveToXY(const coord_t& coords) {

	COORD pos = { static_cast<SHORT>(coords.x), static_cast<SHORT>(coords.y) };

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void Render::Utils::SetTextColor(WORD color) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}