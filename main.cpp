#include <iostream>
#include <cstdlib>

// proj libs
#include "Engine/Renderer.h"



int main() {

	setlocale(0, "rus");
	//std::cout << "Some text in the console" << std::endl;

	Renderer gameRenderer;

	gameRenderer.run();

	system("pause>nul");
	return 0;
}