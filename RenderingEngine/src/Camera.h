#pragma once

#include <GLM/glm.hpp>

class Camera
{
private:
	glm::vec3 position;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	bool dirty;
public:
	Camera(double fovAngle, int width, int height, double zNear, double zFar);
	~Camera();
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjMatrix() const;
	glm::mat4 getViewProjMatrix() const;
};

