#pragma once
#include "Shader.hpp"
#include "Helper.hpp"
#include "BasicCalculation.hpp"

struct ColourVertex
{
	Vec3 pos;
	ColorVariables col;
};

class RenderEllipse
{
public:
	GLuint drawVariable;
	struct Attributes
	{
		int attributeType;
		int size;
		int type;
		uintptr_t offset;
	};

	RenderEllipse(int _width_Line = 1, GLuint _drawVariable = GL_STATIC_DRAW)
		: width_Line(_width_Line),
		_type(0),
		drawVariable(_drawVariable)
	{
		DefineAttributes({
			{ posAttribute, 3, GL_FLOAT, 0 },
			{ colorAttribute,    4, GL_FLOAT, offsetof(ColourVertex, col) }
			});
	}
	~RenderEllipse();

	void Init();
	void CreateBuffer(const std::vector<ColourVertex>& vert, const std::vector<int>& idx, GLuint type);
	void Draw(glm::mat4& matView, glm::mat4& matProjection);
	void SetViewProjMatrix(glm::mat4& matView, glm::mat4& matProjection);

	void DefineAttributes(std::vector<Attributes> attribList)
	{
		attributes.clear();
		for (const auto& ad : attribList)
		{
			attributes.push_back(ad);
		}
	}

	//void calculateEllipseVertices(std::vector<ColourVertex>& vert, std::vector<int> idx);
	void calculateEllipseVertices(std::vector<ColourVertex>& vert, std::vector<int>& vertIdx,
		float a, float b, float angle, uint32_t Num, float Amp, ColorVariables color);


private:

	enum AttributeType : int
	{
		posAttribute = 0,
		colorAttribute = 1
	};
	std::vector<Attributes> attributes;

	GLuint vb;
	GLuint ib;
	GLuint va;
	std::shared_ptr<Shader> shaderEllipse;

	std::vector<ColourVertex> vertices;
	std::vector<int> index;

	GLuint _type;
	int width_Line;

};
