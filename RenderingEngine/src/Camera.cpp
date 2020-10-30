#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Utils.h"

Camera::Camera(double fovAngle, int width, int height, double zNear, double zFar)
{
	projMatrix = glm::perspective(DegToRad(fovAngle), (double) width / height, zNear, zFar);
	//glm::mat4 proj = glm::ortho(-2.0, 2.0, -1.125, 1.125, -1.0, 1.0);

	//viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -6.0, -15.0));
	viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -0.0, -4.0));
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() const
{
	return projMatrix;
}

glm::mat4 Camera::getViewProjMatrix() const
{
	return getProjMatrix() * getViewMatrix();
}
