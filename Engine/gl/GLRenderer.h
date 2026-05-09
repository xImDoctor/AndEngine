#pragma once

// opengl block
#include <glad/glad.h>
#include "OpenGLWindow.h"

#include <vector>

#include "../IRenderer.h"
#include "../RaycastEngine/RaycastEngine.h"


class GLRenderer : public IRenderer {

	OpenGLWindow& window;
	RaycastEngine raycast;

	// for shaders
	// vertex array object, vertex buffer object 
	GLuint VAO, VBO;
	GLuint shaderProgram;

	static constexpr int RENDER_WIDTH = 960;

	std::vector<float> vertexBuffer; // RENDER_WIDTH * 2 vertexes * 5 floats (x, y, r, g, b)

public:

	GLRenderer(OpenGLWindow& win);
	~GLRenderer();

	bool init();

	void render(const std::vector<std::vector<char>>& map,
		const fcoord_t& playerCoord, float playerAngle, bool useDDA = true) override;


private:

	GLuint compileShader(GLenum type, const char* source);
	GLuint createShaderProgram(const char* vertSrc, const char* fragSrc);

	// convert distance to color (r, g, b), like in console ver
	void distanceToColor(float distance, float& r, float& g, float& b) const;

};