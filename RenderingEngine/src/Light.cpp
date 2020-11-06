#include "Light.h"

//#include <string>

Light::Light(unsigned int lightNum, float w) : lightNum(lightNum)
{
	posDir = glm::vec4{ 0.0, 0.0, 0.0, w };
	ambient = glm::vec3{ 0.0, 0.0, 0.0 };
	diffuse = glm::vec3{ 1.0, 1.0, 1.0 };
	specular = glm::vec3{ 1.0, 1.0, 1.0 };

	constant = 1.0;
	linear = 0.14;
	quadratic = 0.07;
}

Light::~Light()
{
}

void Light::SetPosDir(float x, float y, float z, float w)
{
	posDir.x = x;
	posDir.y = y;
	posDir.z = z;
	posDir.w = w;
}

void Light::SetAmbient(float r, float g, float b)
{
	ambient.r = r;
	ambient.g = g;
	ambient.b = b;
}

void Light::SetDiffuse(float r, float g, float b)
{
	diffuse.r = r;
	diffuse.g = g;
	diffuse.b = b;
}

void Light::SetSpecular(float r, float g, float b)
{
	specular.r = r;
	specular.g = g;
	specular.b = b;
}

void Light::SetAttenuations(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::UpdateShader(std::shared_ptr<Shader> activeShader)
{
	std::string str = "uLight[" + std::to_string(lightNum) + "]";
	activeShader->SetUniformVec4(str + ".position", posDir.x, posDir.y, posDir.z, posDir.w);
	activeShader->SetUniformVec4(str + ".ambient", ambient.r, ambient.g, ambient.b, 1.0f);
	activeShader->SetUniformVec4(str + ".diffuse", diffuse.r, diffuse.g, diffuse.b, 1.0f);
	activeShader->SetUniformVec4(str + ".specular", specular.r, specular.g, specular.b, 1.0f);
	activeShader->SetUniformFloat(str + ".constant_attenuation", constant);
	activeShader->SetUniformFloat(str + ".linear_attenuation", linear);
	activeShader->SetUniformFloat(str + ".quadratic_attenuation", quadratic);
}
