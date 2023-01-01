#pragma once
#include <string>

#include <unordered_map>
#include"glm\glm.hpp"
#include "GL/glew.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	GLuint m_RendererID;
	std::string m_FilePath;

	//caching system for uniforms
	std::unordered_map<std::string, int> m_Uniform_LocationCache;

public:
	Shader();
	Shader(const std::string& filepath);
	~Shader();

	//GL use program
	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
	GLuint getRendererID();

private:
	int GetUniformLoaction(const std::string& name);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint CompileShader(GLenum type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
};