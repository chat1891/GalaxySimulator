#include "RenderEllipse.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Helper.hpp"

RenderEllipse::~RenderEllipse()
{

}

void RenderEllipse::Init()
{
	glGenBuffers(1, &vb);
	GET_GLERROR;

	glGenBuffers(1, &ib);
	GET_GLERROR;

	glGenVertexArrays(1, &va);
	GET_GLERROR;

	shaderEllipse = std::make_shared<Shader>("src/shaders/EllipseShader.shader");
	GET_GLERROR;
}

void RenderEllipse::CreateBuffer(const std::vector<ColourVertex>& vert, const std::vector<int>& idx, GLuint type_)
{
	GET_GLERROR;
	vertices = vert;
	index = idx;
	_type = type_;
	GET_GLERROR;

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColourVertex), vertices.data(), drawVariable);
	GET_GLERROR;

	glBindVertexArray(va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	GET_GLERROR;

	for (const Attributes& attrib : attributes)
	{
		glEnableVertexAttribArray(attrib.attributeType);
		if (attrib.type == GL_INT)
		{
			glVertexAttribIPointer(attrib.attributeType, attrib.size, GL_INT, sizeof(ColourVertex), (GLvoid*)attrib.offset);
		}
		else
		{
			glVertexAttribPointer(attrib.attributeType, attrib.size, attrib.type, GL_FALSE, sizeof(ColourVertex), (GLvoid*)attrib.offset);
		}
	}
	GET_GLERROR;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), index.data(), GL_STATIC_DRAW);
	GET_GLERROR;

	glBindVertexArray(0);
	GET_GLERROR;
}

void RenderEllipse::SetViewProjMatrix(glm::mat4& matView, glm::mat4& matProjection)
{
	shaderEllipse->SetUniformMat4f("viewMat", matView);
	shaderEllipse->SetUniformMat4f("projMat", matProjection);
}

void RenderEllipse::calculateEllipseVertices(std::vector<ColourVertex>& vert, std::vector<int>& vertIdx, float a, float b, float angle, uint32_t Num, float Amp, ColorVariables color)
{
	const int steps = 100;
	const float x = 0;
	const float y = 0;

	float beta = -angle * BasicCalculation::degree_rad;
	float sinbeta = std::sin(beta);
	float cosbeta = std::cos(beta);

	int firstPointIdx = static_cast<int>(vert.size());
	for (int i = 0; i < 360; i += 360 / steps)
	{
		float alpha = i * BasicCalculation::degree_rad;
		float sinalpha = std::sin(alpha);
		float cosalpha = std::cos(alpha);

		GLfloat fx = (GLfloat)(x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta));
		GLfloat fy = (GLfloat)(y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta));

		if (Num > 0)
		{
			fx += (GLfloat)((a / Amp) * std::sin(alpha * 2 * Num));
			fy += (GLfloat)((a / Amp) * std::cos(alpha * 2 * Num));
		}

		vertIdx.push_back((int)vert.size());

		ColourVertex vc = { fx, fy, 0, color.r, color.g, color.b, color.a };
		vert.push_back(vc);
	}

	// Close the loop and reset the element index array
	vertIdx.push_back(firstPointIdx);
	vertIdx.push_back(0xFFFF);
}


void RenderEllipse::Draw(glm::mat4& matView, glm::mat4& matProjection)
{
	GET_GLERROR;
	shaderEllipse->Bind();
	GET_GLERROR;

	SetViewProjMatrix(matView, matProjection);

	GET_GLERROR;
	glEnable(GL_PRIMITIVE_RESTART);
	GET_GLERROR;
	glEnable(GL_BLEND);
	GET_GLERROR;

	glPrimitiveRestartIndex(0xFFFF);

	GET_GLERROR;

	glEnable(GL_PROGRAM_POINT_SIZE);

	glBindVertexArray(va);
	glDrawElements(_type, (int)index.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	GET_GLERROR;

	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	glDisable(GL_PRIMITIVE_RESTART);

	glUseProgram(0);
}
