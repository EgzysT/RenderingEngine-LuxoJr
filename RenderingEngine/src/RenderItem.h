#pragma once

#include "GLM/glm.hpp"

#include "Graphics.h"
#include "Mesh.h"

class Graphics;
class RenderItem
{
private:
	Graphics* graphics;
	Mesh mesh;
	glm::mat4 modelMatrix;
public:
	RenderItem(Graphics* graphics, Mesh mesh);
	void Render(bool isShadowPass);
	void SetPosition(double x, double y, double z);
	void SetRotation(float x, float y, float z);
	void SetScale(double x, double y, double z);
};

