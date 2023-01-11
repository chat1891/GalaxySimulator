#pragma once
#include<GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind(const void* data, unsigned int size, GLuint drawMode) const;
	void Unbind() const;
};