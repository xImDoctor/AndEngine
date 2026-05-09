
// proj libs
#include "proj_tests.h"	// startAlgTest

// direct opengl test
#include "Engine/gl/OpenGlWindow.h"

int main() {

	Engine gameEngine;

	//Test::startAlgTest(gameEngine);


	// start with OpenGLWindow class:
	OpenGLWindow window(640, 480, "Test");

	if (!window.init())	// error msg is in callback now
		return -1;
	
	while (!window.shouldClose()) {
		window.clear();
		window.swapBuffers();

		window.pollEvents();
	}

	std::cout << "[Debug] Using cleanup...\n";
	window.cleanup();
	std::cout << "[Debug] Cleanup completed\n";

	return 0;
}