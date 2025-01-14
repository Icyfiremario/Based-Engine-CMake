#pragma once

// STD
#include <stdexcept>

// OpenGL
#include <gl/GL.h>

#ifndef GL_MAJOR
#define GL_MAJOR 3
#endif // !GL_MAJOR

#ifndef GL_MINOR
#define GL_MINOR 2
#endif // !GL_MINOR


class BasedGL
{
public:
	BasedGL();
	~BasedGL();

	void init();

};