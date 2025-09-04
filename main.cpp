#include <iostream>
#include <cstdlib>

// proj libs
#include "Engine/Renderer.h"


int main() {

	//setlocale(0, "rus");
	system("mode con: cols=120 lines=40");

	Renderer gameRenderer;

	gameRenderer.renderMap();
	//gameRenderer.run();

	system("pause>nul");		// Renderer changes console context that conficts with standard one causing start error... WOW
	return 0;
}