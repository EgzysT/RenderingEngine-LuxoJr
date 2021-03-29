#include "Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

#include <fstream>

#include "Utils.h"
#include "Time.h"

#define CAM_ANIM_JSON_PATH "..\\assets\\camAnim.json"

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

	ReadCamAnimsJSON();
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

	ReadCamAnimsJSON();
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

void Camera::Update()
{
	if (camAnimsIterator == camAnims.end()) return;
	double time = glfwGetTime();
	//double time = Time::GetInstance()->GetCurrentLifetime();
	if (camAnimsIterator->startTime > time) return;
	while (camAnimsIterator->endTime < time) {
		camAnimsIterator++;
		if (camAnimsIterator == camAnims.end()) return;
	}
	if (camAnimsIterator->startTime <= time && camAnimsIterator->endTime >= time) {
		camAnimsIterator->Update();
	}
}

void Camera::ReadCamAnimsJSON()
{
	std::ifstream camAnimsJsonFile;
	camAnimsJsonFile.open(CAM_ANIM_JSON_PATH);
	if (!camAnimsJsonFile.is_open()) {
		std::cout << "ERROR: failed to open camera animation file at " << CAM_ANIM_JSON_PATH << std::endl;
	}
	camAnimsJsonFile >> camAnimsJSON;
	camAnimsJsonFile.close();

	//std::vector<CameraAnimation> camAnims;
	auto root = camAnimsJSON["camAnim"];
	for (json::iterator it = root.begin(); it != root.end(); ++it) {
		double timeInit = it->at("timeInit");
		double timeEnd = it->at("timeEnd");
		glm::vec3 eyeInit(it->at("eyeInit")["x"], it->at("eyeInit")["y"], it->at("eyeInit")["z"]);
		glm::vec3 eyeEnd(it->at("eyeEnd")["x"], it->at("eyeEnd")["y"], it->at("eyeEnd")["z"]);
		glm::vec3 centerInit(it->at("centerInit")["x"], it->at("centerInit")["y"], it->at("centerInit")["z"]);
		glm::vec3 centerEnd(it->at("centerEnd")["x"], it->at("centerEnd")["y"], it->at("centerEnd")["z"]);
		CameraAnimation camAnim{ this, timeInit, timeEnd, eyeInit, eyeEnd, centerInit, centerEnd };
		camAnims.push_back(camAnim);
	}
	camAnimsIterator = camAnims.begin();
}
