#include "RenderStars.hpp"
#include <iostream>
#include <fstream>
#include<sstream>

RenderStars::~RenderStars()
{
}

void RenderStars::Init()
	
{
	// Init Buffers
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	glGenVertexArrays(1, &_vao);

	//init shaders
	shader = std::make_unique<Shader>("src/shaders/Star.shader");
}


void RenderStars::SetCustomUniformVariables()
{	
	shader->SetUniform1i("dustSize", _dustSize);
	shader->SetUniform1i("pertN", _num);
	shader->SetUniform1f("pertAmp", _Amp);
	shader->SetUniform1f("time", _time);
	shader->SetUniform1i("displayFeatures", _displayFeatures);	
}

void RenderStars::SetViewProjMatrix(glm::mat4& matView, glm::mat4& matProjection)
{
	shader->SetUniformMat4f("viewMat", matView);
	shader->SetUniformMat4f("projMat", matProjection);
}


void RenderStars::CreateBuffer(const std::vector<StarVertex>& vert, const std::vector<int>& idx, GLuint type)  noexcept(false)
{
	_vertices = vert;
	_idx = idx;
	_type = type;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(StarVertex), _vertices.data(), _bufferMode);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	// Set up vertex buffer array
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	SetVertexAttrib();

	// Set up index buffer array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _idx.size() * sizeof(int), _idx.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void RenderStars::Draw(glm::mat4& matView, glm::mat4& matProjection)
{
	shader->Bind();
	SetViewProjMatrix(matView, matProjection);
	SetCustomUniformVariables();

	EnableBlendFunc();
	BindVertexArrayObj();


	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);

	glUseProgram(0);
}
void RenderStars::EnableBlendFunc()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, _blendFunc);
	glBlendEquation(_blendEquation);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void RenderStars::BindVertexArrayObj()
{
	glBindVertexArray(GetVertexArrayObject());
	glDrawElements(GetPrimitiveType(), GetArrayElementCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}

void RenderStars::SetVertexAttrib()
{
	// Set up vertex buffer attributes
	for (const AttributeDefinition& attrib : _attributes)
	{
		glEnableVertexAttribArray(attrib.attribIdx);
		if (attrib.type == GL_INT)
		{
			glVertexAttribIPointer(attrib.attribIdx, attrib.size, GL_INT, sizeof(StarVertex), (GLvoid*)attrib.offset);
		}
		else
		{
			glVertexAttribPointer(attrib.attribIdx, attrib.size, attrib.type, GL_FALSE, sizeof(StarVertex), (GLvoid*)attrib.offset);
		}
	}
}
