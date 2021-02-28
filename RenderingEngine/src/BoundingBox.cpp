#include "BoundingBox.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

BoundingBox::BoundingBox(glm::vec3 boundingMin, glm::vec3 boundingMax)
	: max(boundingMax), min(boundingMin)
{
	glm::vec3 color(0.7, 0.3, 0.0);

	std::vector<float> verts = {
		//front
		boundingMin.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMin.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		//back
		boundingMin.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		//top
		boundingMin.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMin.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMax.y, boundingMax.z, color.r, color.g, color.b,
		//bottom
		boundingMin.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMin.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMin.z, color.r, color.g, color.b,
		boundingMax.x, boundingMin.y, boundingMax.z, color.r, color.g, color.b

		////front
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		////back
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		////top
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		////bottom
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMax.z
	};

	vbo = std::make_shared<VertexBuffer>(&verts[0], sizeof(float) * verts.size());
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

void BoundingBox::Render(VertexBuffer* vbo)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

	vbo->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);					// position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const GLvoid*)12);	// colors
	glDrawArrays(GL_LINES, 0, 32);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
