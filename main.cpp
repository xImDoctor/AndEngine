#include <iostream>
#include <cstdlib>
#include <ctime>

// proj libs
#include "Engine/Renderer.h"


int main() {

	//setlocale(0, "rus");
	srand(static_cast<unsigned int>(time(NULL)));

	Renderer gameRenderer;

	gameRenderer.renderMap();
	//gameRenderer.run();

	system("pause>nul");
	return 0;
}