#include "RenderStars.hpp"
#include <iostream>
#include <fstream>
#include<sstream>

RenderStars::RenderStars(GLuint blendEquation, GLuint blendFunc, GLuint DrawMode) //GL_STATIC_DRAW
	:_num(0)
	, _dustSize(5)
	, _Amp(0)
	, _time(0)
	, _blendFunc(blendFunc)
	, _blendEquation(blendEquation)
	, _bufferMode(DrawMode)
	, _vertices()
	, _idx()
	, _type(0)
{
	ConstructAttributesLayout();
}

RenderStars::~RenderStars()
{
}

void RenderStars::ConstructAttributesLayout()
{
	_layout.ClearLayoutElements();
	_layout.Push<float>(1);//Theta0
	_layout.Push<float>(1);//VelTheta
	_layout.Push<float>(1);//TiltAngle
	_layout.Push<float>(1);//a
	_layout.Push<float>(1);//b
	_layout.Push<int>(1);//type
	_layout.Push<float>(1);//excentricity
	_layout.Push<float>(4);//color

}

void RenderStars::UpdateStarVariables(float time, int num, float amp, int dustSize, int displayFeatures)
{
	_num = num;
	_Amp = amp;
	_time = time;
	_dustSize = dustSize;
	_displayFeatures = displayFeatures;
}

void RenderStars::Init()
{
	// Init Buffers
	_va = std::make_shared<VertexArray>();
	_vb = std::make_shared<VertexBuffer>();
	_ib = std::make_shared<IndexBuffer>();

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

	_vb->Bind(_vertices.data(),_vertices.size() * sizeof(StarVertex), _bufferMode);

	_va->Bind();
	_va->AddBuffer(*_vb,_layout);

	_ib->Bind((unsigned int*)_idx.data(), _idx.size());

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
	glBindVertexArray(_va->getRendererID());
	glDrawElements(GetPrimitiveType(), GetArrayElementCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}