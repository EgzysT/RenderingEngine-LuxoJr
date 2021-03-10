#pragma once

#include <memory>

#include "GLM/glm.hpp"
#include "VertexBuffer.h"

// Axis-Aligned Bounding Box
class BoundingBox
{
public:
	glm::vec3 min;
	glm::vec3 max;
	std::shared_ptr<VertexBuffer> vbo;

	BoundingBox():vbo(nullptr){}
	BoundingBox(glm::vec3 boundingMin, glm::vec3 boundingMax, glm::vec3 color);
	glm::vec3 calculateCenter();
	glm::vec3 calculateDimensions();

	bool containsPoint(glm::vec3 point);
	bool containsRegion(BoundingBox bb);
	bool intersectsWith(BoundingBox bb);

	void Render();
};

