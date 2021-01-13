#pragma once

#include <memory>

#include "GLM/glm.hpp"

#include "Graphics.h"
#include "Mesh.h"

class Graphics;
class RenderItem
{
private:
	Graphics* graphics;
	std::shared_ptr<Mesh> mesh;
	glm::mat4 modelMatrix;
	glm::vec3 rotationAxis;
	float rotationSpeed;
public:
	RenderItem(Graphics* graphics, std::string meshString);
	void Render(bool isShadowPass);
	void Update(double deltaTime);
	void SetPosition(double x, double y, double z);
	void SetRotation(float x, float y, float z);
	void SetScale(double x, double y, double z);
};

