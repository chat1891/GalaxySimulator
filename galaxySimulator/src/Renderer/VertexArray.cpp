#include "VertexArray.hpp"
#include"../Helper.hpp"
#include "VertexBufferLayout.hpp"
#include <VertexBuffer.hpp>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

void VertexArray::AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
{
	//positions, texture, normal etc
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		if (element.type == GL_INT)
		{
			glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(), (const void*)offset);
		}
		else
		{
			glVertexAttribPointer(i, element.count, element.type, GL_FALSE, layout.GetStride(), (const void*)offset);
		}
		//GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

unsigned int VertexArray::getRendererID() const
{
	return m_RendererID;
}
