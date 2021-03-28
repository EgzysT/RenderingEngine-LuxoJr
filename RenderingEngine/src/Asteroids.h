#pragma once

#include <vector>
#include "GLM/glm.hpp"

#include "RenderItem.h"

class Asteroids
{
public:
	Asteroids(std::vector<RenderItem>* renderItems);
	~Asteroids();

	void CreateModelMatrices();
	void ConfigureInstancedArray();
	void Render();
	void RenderInstanced();
	//void RenderBoundingBoxes();
private:
	std::vector<RenderItem>* renderItems;
	glm::mat4* modelMatrices;
};

