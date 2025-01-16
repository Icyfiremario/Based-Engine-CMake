#pragma once

// STD
#include <stdexcept>

// Local
#include "../renderer.h"


class BasedGL : public Renderer
{
	public:

		void init(GLFWwindow* window);
		void render();
		void cleanup();

		virtual ~BasedGL() = default;
};