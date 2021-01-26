#pragma once

#include "GLM/glm.hpp"

// Axis-Aligned Bounding Box
class BoundingBox
{
public:
	glm::vec3 min;
	glm::vec3 max;

	BoundingBox(){}
	BoundingBox(glm::vec3 boundingMin, glm::vec3 boundingMax);
	glm::vec3 calculateCenter();
	glm::vec3 calculateDimensions();

	bool containsPoint(glm::vec3 point);
	bool containsRegion(BoundingBox bb);
	bool intersectsWith(BoundingBox bb);
};

