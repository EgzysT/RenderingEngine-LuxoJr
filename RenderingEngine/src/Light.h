#pragma once

#include <GLM/glm.hpp>
#include "Shader.h"
#include <memory>

class Light
{
private:
	unsigned int lightNum;
	glm::vec4 posDir;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//attenuation
	float constant;
	float linear;
	float quadratic;
public:
	Light(unsigned int lightNum, float w);
	~Light();
	void SetPosDir(float x, float y, float z, float w);
	void SetAmbient(float r, float g, float b);
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(float r, float g, float b);
	void SetAttenuations(float constant, float linear, float quadratic);
	void UpdateShader(std::shared_ptr<Shader> activeShader);
};

class PointLight : public Light {
public:
	PointLight(unsigned int lightNum) : Light(lightNum, 1.0) {}
	void SetPosition(float x, float y, float z) { SetPosDir(x, y, z, 1.0); }
};

class DirectionalLight : public Light {
public:
	DirectionalLight(unsigned int lightNum) : Light(lightNum, 0.0) {}
	void SetDirection(float x, float y, float z) { SetPosDir(x, y, z, 0.0); }
};

