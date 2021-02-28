#pragma once

#include <memory>

#include "GLM/glm.hpp"

#include "Graphics.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "BoundingBox.h"

class Graphics;
class RenderItem
{
private:
	Graphics* graphics;
	std::shared_ptr<Mesh> mesh;
	glm::mat4 modelMatrix;
	//glm::vec3 rotationAxis;
	//float rotationSpeed;

	glm::vec3 boundingMin;
	glm::vec3 boundingMax;
	std::shared_ptr<VertexBuffer> boundingBoxVBO;

public:
	BoundingBox aabb;

	RenderItem(Graphics* graphics, std::string meshString);
	void Render(bool isShadowPass);
	void RenderBoundingBox();
	void Update(double deltaTime);
	void SetPosition(double x, double y, double z);
	void SetRotation(float x, float y, float z);
	void SetScale(double x, double y, double z);
	void CalculateBoundingBox();
};

