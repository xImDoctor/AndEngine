#include "Engine.h"


// Method makes GLWindow (wrapper above OpenGLWindow class)
bool Engine::createWindow(int width, int height, const std::string& title) {

	glWindow = OpenGLWindow(width, height, title);
	return glWindow.init();
}

void Engine::removeWindow() {
	renderer.reset();	// cleanup rendrer context before window destruction
	glWindow.cleanup();
}

// Game cycle running
void Engine::run(bool useOpenGLRenderer, bool use_DDA_render) {

	// init choosen renderer (gl, console)
	if (useOpenGLRenderer) {
		
		if (!getGLWindowStatus()) {
			std::cerr << "[Error] Use createWindow() before game-loop running with GL renderer\n";
			return;
		}

		glWindow.setVSync(true);
		renderer = std::make_unique<GLRenderer>(glWindow);
	}
	else {
		renderer = std::make_unique<ConsoleRenderer>();

		system("cls");	// clear window before game drawing (console only)
	}

	auto lastTime = std::chrono::high_resolution_clock::now();
	isRunning = true;
	
	while (isRunning) {

		// every loop start check if window should be closed and process events for best impact
		if (useOpenGLRenderer) {
			if (glWindow.shouldClose()) break; // not isRunning = false to return immediately

			glWindow.pollEvents(); // event processing
		}


		// compute deltaTime value
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		deltaTime = std::fmin(deltaTime, MAX_DELTA_TIME);

		// controller processing
		controller.update();
		controller.processInput(generator.getMap(), Render::Objects::PATH, 
			playerCoord, playerAngle, MOVEMENT_SPEED, ROTATION_SPEED, deltaTime);

		// stop render when Esc pressed
		if (controller.isEscPressed())
			isRunning = false;

		// scene rendering with choosen mode
		renderer->render(generator.getMap(), playerCoord, playerAngle, use_DDA_render);

		// sleep time with delay to stabilize frames (~FPS) - actual only for console renderer
		// GL uses VSync instead
		if (!useOpenGLRenderer) {
			auto frameEndTime = std::chrono::high_resolution_clock::now();
			auto frameTime = std::chrono::duration<float>(frameEndTime - currentTime).count();

			if (frameTime < FRAME_TIME) {
				int sleepingMs = static_cast<int>((FRAME_TIME - frameTime) * 1000.0f);
				Sleep(sleepingMs);
			}
		}
	}
}


// normalize angle to [0; 2pi]
void Engine::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}