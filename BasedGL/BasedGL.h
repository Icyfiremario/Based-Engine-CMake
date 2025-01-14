#pragma once

// STD
#include <stdexcept>

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>

// GLFW
#include <GLFW/glfw3.h>

class BasedGL
{
public:
	BasedGL(GLFWwindow* window);
	~BasedGL();

	void init();
};