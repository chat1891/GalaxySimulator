#pragma once
#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef DEBUG
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif

