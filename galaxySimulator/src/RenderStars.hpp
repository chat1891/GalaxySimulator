#include "BasicCalculation.hpp"
#include "Shader.hpp"
#include "Star.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cstdint>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include<memory>




struct StarVertex
{
	Star star;
	ColorVariables colour;
};

class RenderStars
{
public:
	~RenderStars();
protected:
	struct AttributeDefinition
	{
		int attribIdx;
		int size;
		int type;
		uintptr_t offset;
	};

	GLuint _bufferMode;

	int GetPrimitiveType() const
	{
		return _type;
	}

	int GetArrayElementCount() const
	{
		return (int)_idx.size();
	}

	GLuint GetVertexArrayObject() const
	{
		return _vao;
	}	

	std::unique_ptr<Shader> shader;

	std::vector<AttributeDefinition> _attributes;



	std::vector<StarVertex> _vertices;
	std::vector<int> _idx;

	GLuint _type;
	GLuint _vbo;
	GLuint _ibo;
	GLuint _vao;


public:
	RenderStars(GLuint blendEquation, GLuint blendFunc, GLuint DrawMode) //GL_STATIC_DRAW
		:_num(0)
		, _dustSize(0)
		, _Amp(0)
		, _time(0)
		, _blendFunc(blendFunc)
		, _blendEquation(blendEquation)
		,_bufferMode(DrawMode)
		,_vbo(0)
		, _ibo(0)
		, _vao(0)
		, _vertices()
		, _idx()
		, _type(0)
	{
		InitAllAttributes();
	}

	void InitAllAttributes()
	{
		_attributes.clear();
		AttributeDefinition attrib = { attTheta0,1, GL_FLOAT, offsetof(Star, theta0) };
		AddAttribute(attrib);
		attrib = { attVelTheta,      1, GL_FLOAT, offsetof(Star, velTheta) };
		AddAttribute(attrib);
		attrib = { attTiltAngle,     1, GL_FLOAT, offsetof(Star, tiltAngle) };
		AddAttribute(attrib);
		attrib = { attSemiMajorAxis, 1, GL_FLOAT, offsetof(Star, a) };
		AddAttribute(attrib);
		attrib = { attSemiMinorAxis, 1, GL_FLOAT, offsetof(Star, b) };
		AddAttribute(attrib);
		attrib = { attType,          1, GL_INT, offsetof(Star, type) };
		AddAttribute(attrib);
		attrib = { attColor,         4, GL_FLOAT, offsetof(StarVertex, colour) };
		AddAttribute(attrib);
	}

	void AddAttribute(AttributeDefinition attrib)
	{
		_attributes.push_back(attrib);
	}

	void UpdateStarVariables(float time, int num, float amp, int dustSize, int displayFeatures)
	{
		_num = num;
		_Amp = amp;
		_time = time;
		_dustSize = dustSize;
		_displayFeatures = displayFeatures;
	}

	void Init();
	void Draw(glm::mat4& matView, glm::mat4& matProjection);

	void SetCustomUniformVariables();
	void SetViewProjMatrix(glm::mat4& matView, glm::mat4& matProjection);
	void CreateBuffer(const std::vector<StarVertex>& vert, const std::vector<int>& idx, GLuint type)  noexcept(false);
	void EnableBlendFunc();
	void BindVertexArrayObj();
	void SetVertexAttrib();

private:

	enum AttributeIdx : int
	{
		attTheta0 = 0,
		attVelTheta,
		attTiltAngle,
		attSemiMajorAxis,
		attSemiMinorAxis,
		attType,
		attColor
	};

	// parameters for density wave computation
	int _num;
	int _dustSize;
	float _Amp;
	float _time;
	GLuint _blendFunc;
	GLuint _blendEquation;
	int _displayFeatures;
};