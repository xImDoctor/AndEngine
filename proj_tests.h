#pragma once

#include "Engine/Engine.h"


namespace Test {
	namespace Commands {
		constexpr const char* START_RAYCAST = "start raycast";
		constexpr const char* START_DDA = "start dda";

		constexpr const char* SET_CONSOLE_MODE = "use console";
		constexpr const char* SET_GL_MODE = "use gl";

		constexpr const char* INFO = "info";
		constexpr const char* EXIT = "exit";

		constexpr const char* CHANGE_SEED = "change seed";
		constexpr const char* SHOW_MAP = "show map";

		//constexpr const char* SHOW_TRIG = "show trig";	// show trigonometry (sin, cos) tables
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

			//  just rederer update block
			std::cout << "Renderer update: now implemented two render modes: console (old) and OpenGL graphics (new)." << std::endl;
			std::cout << "Program uses old console one as default but you can change it with command below" << std::endl;

			std::cout << "Type one of the following commands:" << std::endl;

			std::cout << std::endl << "Start rendering (console by default):" << std::endl;
			std::cout << "start raycast - starts rendering with step-based raycast (normal)" << std::endl;
			std::cout << "start dda - starts rendering with DDA Raycasting algorithm (works faster)" << std::endl;

			std::cout << std::endl << "Change renderer mode:" << std::endl;
			std::cout << "use console - sets console renderer (default)" << std::endl;
			std::cout << "use gl - sets window renderer made with OpenGL graphics, not console" << std::endl;

			std::cout << std::endl << "Map generator:" << std::endl;
			std::cout << "change seed - opens seed settings" << std::endl;
			std::cout << "show map - displays pre-generated game map" << std::endl;
			
			std::cout << std::endl << "How to use and logout:" << std::endl;
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

		bool useGLRenderer = false;

		while (1) {

			system("cls");
			showStartAlgInfo(InfoType::General);

			Render::Utils::SetTextColor(Render::Objects::Colors::Yellow);
			std::cout << std::endl << "Your input: ";
			Render::Utils::setDefaultColor();

			inputBuf.clear();
			std::getline(std::cin, inputBuf);


			// limit string size after input
			if (inputBuf.size() > MAX_START_INPUT_SIZE)
				inputBuf.resize(MAX_START_INPUT_SIZE);

			// run engine block
			if (inputBuf == Commands::START_RAYCAST) {
				engine.run(useGLRenderer);
			}
			else if (inputBuf == Commands::START_DDA) {
				engine.run(useGLRenderer, true);	// second param: use DDA flag enabled
			}
			// set renderer (console, gl), just turns on/off gl renderer use
			else if (inputBuf == Commands::SET_CONSOLE_MODE) {
				useGLRenderer = false;
			}
			else if (inputBuf == Commands::SET_GL_MODE) {
				useGLRenderer = true;
			}
			/* commented because precompute method moved to RaycastEngine (not accessable now)
			else if (inputBuf == Commands::SHOW_TRIG) {
				 engine.showPrecomputedTrigVals();
				showStartAlgInfo(InfoType::Continue);
			}*/
			// other stuff (seed, map, info, exit)
			else if (inputBuf == Commands::CHANGE_SEED) {
				unsigned int newSeed;

				system("cls");
				std::cout << "Current map seed=" << engine.getMapSeed() << std::endl;
				std::cout << "Input new integer seed value (0 to exit without changes):" << std::endl;
				std::cout << "Seed="; 
				
				if (StreamUtils::safeInputUInt(newSeed)) {

					if (!newSeed)
						continue;

					engine.useMapGenerator().changeMapSeed(newSeed);
					std::cout << "Seed successfully changed!" << std::endl;
				}
				else
					std::cerr << "Invalid input! Enter a valid positive integer.\n";

				showStartAlgInfo(InfoType::Continue);
			}
			else if (inputBuf == Commands::SHOW_MAP) {
				system("cls");
				std::cout << "Current map seed: " << engine.getMapSeed() << std::endl;
				std::cout << "Pre-generated game map:" << std::endl;
				engine.useMapGenerator().showMap();
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