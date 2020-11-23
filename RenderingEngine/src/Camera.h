#pragma once

#include <GLM/glm.hpp>

class Camera
{
private:
	glm::vec3 eye;
	glm::vec3 center;
	const glm::vec3 up;
	const double fov;
	const double aspect;
	const glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	bool dirty;

public:
	Camera(double fovAngle, double aspect, double zNear, double zFar);
	Camera(double fovy, double aspect, double zNear, double zFar, glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	~Camera();
	void rotate(double xoffset, double yoffset);
	void dolly(double offset);
    void pan(double xoffset, double yoffset);
	void moveLocalX(double offset);
	void moveLocalY(double offset);
	void moveLocalZ(double offset);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix() const;
	glm::mat4 getViewProjMatrix();
};

