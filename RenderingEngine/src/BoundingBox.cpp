#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 boundingMin, glm::vec3 boundingMax)
	: max(boundingMax), min(boundingMin)
{
}

glm::vec3 BoundingBox::calculateCenter()
{
	return (min + max) / 2.0f;
}

glm::vec3 BoundingBox::calculateDimensions()
{
	//return glm::vec3(glm::abs(max.x - min.x), glm::abs(max.y - min.y), glm::abs(max.z - min.z));
	return max - min;
}

bool BoundingBox::containsPoint(glm::vec3 point)
{
	return (point.x >= min.x) && (point.x <= max.x) &&
		(point.y >= min.y) && (point.y <= max.y) &&
		(point.z >= min.z) && (point.z <= max.z);
}

bool BoundingBox::containsRegion(BoundingBox bb)
{
	//return (bb.min.x >= min.x) && (bb.max.x <= max.x) &&
	//	(bb.min.y >= min.y) && (bb.max.y <= max.y) &&
	//	(bb.min.z >= min.z) && (bb.max.z <= max.z);
	return containsPoint(bb.min) && containsPoint(bb.max);
}

bool BoundingBox::intersectsWith(BoundingBox bb)
{
	glm::vec3 rad = calculateDimensions() / 2.0f;		//"radius" of box
	glm::vec3 radBB = bb.calculateDimensions() / 2.0f;
	glm::vec3 center = calculateCenter();
	glm::vec3 centerBB = bb.calculateCenter();
	glm::vec3 dist = glm::abs(center - centerBB);

	for (int i = 0; i < 3; i++) {
		if (dist[i] > rad[i] + radBB[i]) {	// Boxes don't overlap on this axis
			return false;
		}
	}
	return true;
}
