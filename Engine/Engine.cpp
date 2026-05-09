#include "Engine.h"


// Game cycle running
void Engine::run(bool useOpenGLRenderer, bool use_DDA_render) {

	system("cls");			// clear window before game drawing

	
	// init choosen renderer (gl, console)
	if (useOpenGLRenderer)
		renderer = std::make_unique<GLRenderer>(glWindow);
	else 
		renderer = std::make_unique<ConsoleRenderer>();


	auto lastTime = std::chrono::high_resolution_clock::now();
	
	isRunning = true;
	while (isRunning) {

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

		auto frameEndTime = std::chrono::high_resolution_clock::now();
		auto frameTime = std::chrono::duration<float>(frameEndTime - currentTime).count();

		// sleep time with delay to stabilize frames (~FPS)
		if (frameTime < FRAME_TIME) {
			int sleepingMs = static_cast<int>((FRAME_TIME - frameTime) * 1000.0f);
			Sleep(sleepingMs);
		}
	}
}


// normalize angle to [0; 2pi]
void Engine::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}