#include "OpenGLWindow.h"


OpenGLWindow::OpenGLWindow(int w, int h, const std::string& title) :
	width(w), height(h), windowTitle(title), window(nullptr), isInitialized(false) {
}


bool OpenGLWindow::init() {

	// set error callback
	glfwSetErrorCallback(errorCallback);

	// init glfw
	if (!glfwInit()) {
#ifdef _DEBUG
		std::cerr << "[Error] Failed to init the GLFW\n";
#endif
		return false;
	}

#ifdef _DEBUG
	std::cout << "[Success] GLFW is initialized\n";
#endif

	// setup Opengl
	// opengl core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// additional window settings
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA (smoothing)


	// create window
	window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

	if (!window) {
#ifdef _DEBUG
		std::cerr << "[Error] Failed to create a window\n";
#endif
		glfwTerminate();
		return false;
	}
#ifdef _DEBUG
	std::cout << "[Success] Window is created\n";
#endif

	// Make OpenGL context for this window
	glfwMakeContextCurrent(window);


	// load OpenGL function pointers via GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef _DEBUG
		std::cerr << "[Error] Failed to init GLAD\n";
#endif	
		glfwTerminate();

		isInitialized = false;
		return isInitialized;
	}
#ifdef _DEBUG
	std::cout << "[Success] GLAD is initialized\n";
#endif

	// save pointer for callbacks
	glfwSetWindowUserPointer(window, this);

	// Set callbacks to change window size
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	isInitialized = true;
	return isInitialized;
}



// glfw callbacks
void OpenGLWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void OpenGLWindow::errorCallback(int error, const char* description) {
	std::cerr << "[Error]" << " Code " << error << "\nDescription: " 
													<< description << "\n";
}

// other methods
void OpenGLWindow::cleanup() {

	if (window) {
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwTerminate();
}

bool OpenGLWindow::shouldClose() const {
	return glfwWindowShouldClose(window);
}

void OpenGLWindow::pollEvents() {
	glfwPollEvents();
}

void OpenGLWindow::swapBuffers() {
	glfwSwapBuffers(window);
}


void OpenGLWindow::clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}


// setters
void OpenGLWindow::setVSync(bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
}

// mods: GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED
void OpenGLWindow::setCursorMode(int mode) {
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void OpenGLWindow::setWindowTitle(const std::string& newTitle) {
	windowTitle = newTitle;

	glfwSetWindowTitle(window, windowTitle.c_str());
}

float OpenGLWindow::getAspectRatio() const {
	return static_cast<float>(width) / static_cast<float>(height);
}
