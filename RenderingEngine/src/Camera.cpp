#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Utils.h"

Camera::Camera(double fovy, double aspect, double zNear, double zFar) :
	fov(fovy),
	aspect(aspect),
	projMatrix(glm::perspective(fovy, aspect, zNear, zFar)),
	eye(glm::vec3(0.0f, 0.0f, 4.0f)),
	center(glm::vec3()),
	up(glm::vec3(0.0f, 1.0f, 0.0f)),
	dirty(true) 
{
	viewMatrix = glm::lookAt(eye, center, up);
};

Camera::Camera(double fovy, double aspect, double zNear, double zFar, glm::vec3 eye, glm::vec3 center, glm::vec3 up) :
	fov(fovy),
	aspect(aspect),
	projMatrix(glm::perspective(fovy, aspect, zNear, zFar)),
	eye(eye),
	center(center),
	up(up),
	dirty(true),
	viewMatrix(glm::lookAt(eye, center, up)) 
{
	viewMatrix = glm::lookAt(eye, center, up);
};

Camera::~Camera()
{
}

void Camera::rotate(double xoffset, double yoffset) {
	auto tan_fov = glm::tan(fov / 2);

	auto xangle = glm::atan(xoffset * tan_fov * aspect) * 2;
	auto yangle = glm::atan(yoffset * tan_fov) * 2;

	auto direction = glm::normalize(center - eye);
	auto radius = glm::length(center - eye);

	auto phi = glm::atan(direction.z, direction.x) + xangle;
	auto theta = acos(direction.y) + yangle;
	theta = glm::clamp(theta, 0.001, glm::pi<double>() - 0.001);

	auto x = glm::sin(theta) * glm::cos(phi) * radius;
	auto y = glm::cos(theta) * radius;
	auto z = glm::sin(theta) * glm::sin(phi) * radius;

	eye = center - glm::vec3(x, y, z);
	dirty = true;
}

void Camera::dolly(double offset) {
	auto direction = glm::normalize(center - eye);
	auto radius = glm::max(0.001, glm::length(center - eye) * (1 - offset));

	eye = center - (float)radius * direction;
	dirty = true;
}

void Camera::pan(double xoffset, double yoffset) {
	auto tan_fov = glm::tan(fov / 2) * 2;
	auto radius = glm::length(center - eye);

	auto delta_x = xoffset * radius * tan_fov * aspect;
	auto delta_y = yoffset * radius * tan_fov;

	auto direction = glm::normalize(center - eye);
	auto right = glm::normalize(glm::cross(up, direction));
	auto up = glm::normalize(glm::cross(direction, right));

	auto t = glm::vec3(delta_x * right.x + delta_y * up.x,
		delta_x * right.y + delta_y * up.y,
		delta_x * right.z + delta_y * up.z);

	eye += t;
	center += t;
	dirty = true;
}

void Camera::moveLocalX(double offset)
{
	pan(offset, 0);
}

void Camera::moveLocalY(double offset)
{
	pan(0, offset);
}

void Camera::moveLocalZ(double offset)
{
	double tan_fov = glm::tan(fov / 2) * 2;
	float radius = glm::length(center - eye);

	float delta_z = offset * radius * tan_fov;

	auto direction = glm::normalize(center - eye);

	glm::vec3 t = direction * delta_z;
	center += t;
	eye += t;
	dirty = true;
}

glm::mat4 Camera::getViewMatrix()
{
	if (dirty) {
		viewMatrix = glm::lookAt(eye, center, up);
	}
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() const
{
	return projMatrix;
}

glm::mat4 Camera::getViewProjMatrix()
{
	return getProjMatrix() * getViewMatrix();
}
