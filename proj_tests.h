#pragma once

#include "Engine/Renderer.h"


void showStartAlgInfo(const char* infoPart = "general") {

	if (infoPart == "continue") {
		std::cout << "Press any key to continue" << std::endl;
		system("pause>nul");
		return;
	}

	if (infoPart == "general") {

		std::cout << "AndEngine renderer test." << std::endl;
		std::cout << "Type one of the following commands:" << std::endl;

		std::cout << "start, start raycast - starts console rendering with step-based raycast" << std::endl;
		std::cout << "start dda - starts console rendering with DDA Raycasting algorithm" << std::endl; // faster one
		std::cout << "info - information about in-scene data and controls" << std::endl;
		std::cout << "exit - stop this application" << std::endl;
	}
	else if (infoPart == "controls") {

		std::cout << "Now you can watch simple raycast rendering visualization with two algoritm realizations: step-based, DDA" <<
			"\nDemonstration starts on the randomly generated map" << std::endl << std::endl;
		std::cout << "Controls:\n- W/S - move forward/backward\n- A/D - rotate camera to left/right\n- Esc - stop (leave) rendering" << std::endl << std::endl;
		std::cout << "Other info:\n- Wall symbols and colors are different based on distance towards it\n-There is the minimap of created scene in the upper-right corner" << std::endl;

		showStartAlgInfo("continue");
	}
}


// Starts engine rendering with one of the choosen algorithms (step-based, dda)
void startAlgTest(Renderer& renderer) {

	const std::size_t MAX_START_INPUT_SIZE = 14;
	std::string inputBuf;

	while (1) {

		if (inputBuf.size() > MAX_START_INPUT_SIZE)
			inputBuf.resize(MAX_START_INPUT_SIZE);

		system("cls");
		showStartAlgInfo("general");

		std::cout << "Input: \n";
		std::getline(std::cin, inputBuf);

		if (inputBuf == "start" || inputBuf == "start raycast") {
			renderer.run();
		}
		else if (inputBuf == "start dda" || inputBuf == "start DDA") {
			std::cout << "DDA is not implemented now" << std::endl;
			showStartAlgInfo("continue");
		}
		else if (inputBuf == "info") {
			system("cls");
			showStartAlgInfo("controls");
		}
		else if (inputBuf == "exit") {
			std::cout << "App stopped, goodbye!\n";
			exit(0);	// instead of break to exit whole app, not just this method
		}
	}
}
