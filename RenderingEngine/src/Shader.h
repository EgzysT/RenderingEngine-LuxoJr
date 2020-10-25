#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>

class Shader
{
private:
	unsigned int rendererID;
	std::string vertFilePath;
	std::string fragFilePath;
	std::unordered_map<std::string, GLint> uniformLocationCache;
	GLint getUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
public:
	//Constructor does not bind the shader program, only compiles the shader program, you still need to bind to use!
	Shader(const std::string& vertFilePath, const std::string& fragFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set Uniforms
	void SetUniformVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformInteger(const std::string& name, int value);
	void SetUniformMatrix4(const std::string& name, glm::mat4 matrix);
};

