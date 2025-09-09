#pragma once

#include "Engine/Engine.h"


namespace Test {
	namespace Commands {
		constexpr const char* START = "start";
		constexpr const char* START_RAYCAST = "start raycast";
		constexpr const char* START_DDA = "start dda";
		constexpr const char* INFO = "info";
		constexpr const char* EXIT = "exit";
	}

	enum class InfoType {
		General,
		Controls,
		Continue
	};


	// Displays info panels of depended InfoType inside of startAlgTest()
	void showStartAlgInfo(InfoType type = InfoType::General) {				//const char* infoPart = "general"

		if (type == InfoType::Continue) {
			std::cout << "Press any key to continue" << std::endl;
			system("pause>nul");
			return;
		}

		if (type == InfoType::General) {

			std::cout << "AndEngine renderer test." << std::endl;
			std::cout << "Type one of the following commands:" << std::endl;

			std::cout << "start, start raycast - starts console rendering with step-based raycast" << std::endl;
			std::cout << "start dda - starts console rendering with DDA Raycasting algorithm" << std::endl; // faster one
			std::cout << "info - information about in-scene data and controls" << std::endl;
			std::cout << "exit - stop this application" << std::endl;
		}
		else if (type == InfoType::Controls) {

			std::cout << "Now you can watch simple raycast rendering visualization with two algoritm realizations: step-based, DDA" <<
				"\nDemonstration starts on the randomly generated map" << std::endl << std::endl;
			std::cout << "Controls:\n- W/S - move forward/backward\n- A/D - rotate camera to left/right\n- Esc - stop (leave) rendering" << std::endl << std::endl;
			std::cout << "Other info:\n- Wall symbols and colors are different based on distance towards it\n-There is the minimap of created scene in the upper-right corner" << std::endl;
			std::cout << "---" << std::endl;
			std::cout << "[CAUTION] Maybe you should minimize your console scale (ctrl+mouse_wheel for example) when rendering starts" <<
				" because render field might be larger then default console size" << std::endl;
			showStartAlgInfo(InfoType::Continue);
		}
	}


	// Starts engine rendering with one of the choosen algorithms (step-based, dda)
	void startAlgTest(Engine& engine) {

		const std::size_t MAX_START_INPUT_SIZE = 14;
		std::string inputBuf;

		while (1) {

			if (inputBuf.size() > MAX_START_INPUT_SIZE)
				inputBuf.resize(MAX_START_INPUT_SIZE);

			system("cls");
			showStartAlgInfo(InfoType::General);

			std::cout << "Input: \n";
			std::getline(std::cin, inputBuf);

			if (inputBuf == Commands::START || inputBuf == Commands::START_RAYCAST) {
				engine.run();
			}
			else if (inputBuf == Commands::START_DDA) {
				std::cout << "DDA is not implemented now" << std::endl;
				showStartAlgInfo(InfoType::Continue);
			}
			else if (inputBuf == Commands::INFO) {
				system("cls");
				showStartAlgInfo(InfoType::Controls);
			}
			else if (inputBuf == Commands::EXIT) {
				std::cout << "App stopped, goodbye!\n";
				exit(0);	// instead of break to exit whole app, not just this method
			}
		}
	}

}