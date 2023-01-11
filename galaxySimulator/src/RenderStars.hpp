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

#include <VertexBufferLayout.hpp>
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

struct StarVertex
{
	Star star;
	ColorVariables colour;
};

class RenderStars
{
public:
	RenderStars(GLuint blendEquation, GLuint blendFunc, GLuint DrawMode);
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

	std::unique_ptr<Shader> shader;
	VertexBufferLayout _layout;

	std::vector<StarVertex> _vertices;
	std::vector<int> _idx;

	GLuint _type;

	std::shared_ptr <VertexArray> _va;
	std::shared_ptr<VertexBuffer> _vb;
	std::shared_ptr<IndexBuffer> _ib;


public:
	void ConstructAttributesLayout();
	void UpdateStarVariables(float time, int num, float amp, int dustSize, int displayFeatures);

	void Init();
	void Draw(glm::mat4& matView, glm::mat4& matProjection);

	void SetCustomUniformVariables();
	void SetViewProjMatrix(glm::mat4& matView, glm::mat4& matProjection);
	void CreateBuffer(const std::vector<StarVertex>& vert, const std::vector<int>& idx, GLuint type)  noexcept(false);
	void EnableBlendFunc();
	void BindVertexArrayObj();

private:
	// parameters for density wave computation
	int _num;
	int _dustSize;
	float _Amp;
	float _time;
	GLuint _blendFunc;
	GLuint _blendEquation;
	int _displayFeatures;
};