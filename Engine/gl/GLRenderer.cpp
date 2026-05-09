#include "GLRenderer.h"

// min shaders (inline made without external files, just to check)
static const char* vertSrc = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 fragColor;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        fragColor = aColor;
    }
)";

static const char* fragSrc = R"(
    #version 330 core
    in vec3 fragColor;
    out vec4 outColor;
    void main() {
        outColor = vec4(fragColor, 1.0);
    }
)";


GLRenderer::GLRenderer(OpenGLWindow& win) : window(win), 
VAO(0), VBO(0), shaderProgram(0), vertexBuffer(RENDER_WIDTH*2*5, 0.0f) 
{ 
	init();
}

GLRenderer::~GLRenderer() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(shaderProgram);
}


void GLRenderer::distanceToColor(float dist, float& r, float& g, float& b) const {
	float t = dist / RaycastEngine::DEPTH; // 0 - too close, 1 - too far
	r = g = b = 1.0f - t;  // brighter (more white) if close, darker if far so same between canals
}


bool GLRenderer::init() {

    // make program with shaders code (inline, above in this file)
    shaderProgram = createShaderProgram(vertSrc, fragSrc);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // alloc memory for max buffer for data changing every frame
    glBufferData(GL_ARRAY_BUFFER,
        vertexBuffer.size() * sizeof(float),
        nullptr, GL_DYNAMIC_DRAW);

    // layout: pos (2 float) + color (3 float) = 5 float, stride = 5 * sizeof(float)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return true;
}


void GLRenderer::render(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float playerAngle, bool useDDA) {

    int vIdx = 0;
    for (int col = 0; col < RENDER_WIDTH; ++col) {

        float rayAngle = (playerAngle - RaycastEngine::FOV / 2.0f)
            + ((float)col / RENDER_WIDTH) * RaycastEngine::FOV;

        float dist = useDDA ? raycast.castRay_dda(map, playerCoord, rayAngle)
                            : raycast.castRay_stepped(map, playerCoord, rayAngle);

        dist *= cosf(rayAngle - playerAngle); // to fish-eye fix

        float wallHeight = 1.0f / dist;       // NDC: height from 0 to 1+
        wallHeight = std::fmin(wallHeight, 1.0f);

        float x = (2.0f * col / RENDER_WIDTH) - 1.0f; // NDC: -1...1

        float r, g, b;
        distanceToColor(dist, r, g, b);

        // top vertex
        vertexBuffer[vIdx++] = x;
        vertexBuffer[vIdx++] = wallHeight;
        vertexBuffer[vIdx++] = r;
        vertexBuffer[vIdx++] = g;
        vertexBuffer[vIdx++] = b;

        // bottom vertex
        vertexBuffer[vIdx++] = x;
        vertexBuffer[vIdx++] = -wallHeight;
        vertexBuffer[vIdx++] = r;
        vertexBuffer[vIdx++] = g;
        vertexBuffer[vIdx++] = b;
    }

    window.clear();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer.size() * sizeof(float), vertexBuffer.data());

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, RENDER_WIDTH * 2);

    window.swapBuffers();
}

// Makes shader object and compiles it
GLuint GLRenderer::compileShader(GLenum type, const char* source) {

    GLuint shader = glCreateShader(type); // make object of shader(GL_VERTEX_SHADER / GL_FRAGMENT_SHADER)
    glShaderSource(shader, 1, &source, nullptr); // load made source

    glCompileShader(shader);

    // check compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {

        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);

        std::cerr << "[Shader error] " << log << "\n";

        glDeleteShader(shader);
        return 0;
    }

    // return if compiled successfully
    return shader;
}

// Links shaders to program
GLuint GLRenderer::createShaderProgram(const char* vertSrc, const char* fragSrc) {

    // compile vertex and fragment shaders
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    // check if both compiled
    if (!vert || !frag) return 0;

    GLuint program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);
    
    glLinkProgram(program); // linking

    // check if lincked successfully
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);

        std::cerr << "[Program link error] " << log << "\n";
    }

    // after linking shaders are in program 
    // sources could be removed yeeah
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}


