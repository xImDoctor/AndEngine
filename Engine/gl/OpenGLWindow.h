#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

class OpenGLWindow {

	GLFWwindow* window;

	int height, width;
	std::string windowTitle;

	bool initialized;

	// glfw callbacks, needs to be inited
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void errorCallback(int error, const char* description);

public:
	OpenGLWindow(int w = 1280, int h = 720, const std::string& windowTitle = "Unnamed Window");
	~OpenGLWindow() { cleanup(); } // uses cleanup() to free resources

	// initialize window and context
	bool init();

	// clear sources
	void cleanup();

	// check if should close the window
	bool shouldClose() const;

	// event processing
	void pollEvents();

	// swap of buffers
	void swapBuffers();

	// clear window func
	void clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);


//getters

	GLFWwindow* getWindow() const { return window; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool getInitStatus() const { return initialized; }

//setters

	void setVSync(bool enabled);
	void setCursorMode(int mode);
	void setWindowTitle(const std::string& newTitle);


// util func (getter)
	float getAspectRatio() const;

};