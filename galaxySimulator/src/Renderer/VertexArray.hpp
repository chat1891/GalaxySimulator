#pragma once

#include "VertexBuffer.hpp"
//#include "VertexBufferLayout.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
	unsigned int getRendererID() const;
};