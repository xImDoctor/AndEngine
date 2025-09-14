#include "Engine.h"


// Game cycle running
void Engine::run(bool use_DDA_render) {

	system("cls");			// clear window before game drawing

	isRunning = true;
	while (isRunning) {

		controller.update();
		controller.processInput(generator.getMap(), Render::Objects::PATH, 
			playerCoord, playerAngle, MOVEMENT_SPEED, ROTATION_SPEED);

		// stop render when Esc pressed
		if (controller.isEscPressed())
			isRunning = false;

		renderer.render(generator.getMap(), playerCoord, playerAngle, use_DDA_render);

		// sleep time with delay to stabilize frames (~FPS)
		Sleep(renderer.getRenderDelay());
	}
}


// normalize angle to [0; 2pi]
void Engine::normalizeAngle(float& angle) {
	MathUtils::Angles::normalizeAngle(angle);
}