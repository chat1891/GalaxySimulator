#pragma once
#include <iostream>
#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef DEBUG
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif

#define GET_GLERROR GLErrors();

static void GLErrors()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[openGL Error]: (" << error << ")" << std::endl;
		//throw std::runtime_error("Error");
	}
}

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[openGL Error]: (" << error << ")" << std::endl;
		return false;
	}
	return true;
}

struct Vec3
{
	float x, y, z;
};

struct ColorVariables
{
	float r, g, b, a;
};


