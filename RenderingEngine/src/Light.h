#pragma once

#include <GLM/glm.hpp>
#include "Shader.h"
#include <memory>

class Light
{
private:
	unsigned int lightNum;
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//attenuation
	float constant;
	float linear;
	float quadratic;
public:
	Light(unsigned int lightNum);
	~Light();
	void SetPosition(float x, float y, float z);
	void SetAmbient(float r, float g, float b);
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(float r, float g, float b);
	void SetAttenuations(float constant, float linear, float quadratic);
	void UpdateShader(std::shared_ptr<Shader> activeShader);
};

