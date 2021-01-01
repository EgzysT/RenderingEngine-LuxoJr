#pragma once

#include <GLM/glm.hpp>
#include <json.hpp>

#include "CameraAnimation.h"

class Camera
{
	using json = nlohmann::json;

public:
	glm::vec3 eye;
	glm::vec3 center;
	bool dirty;
private:
	const glm::vec3 up;
	const double fov;
	const double aspect;
	const glm::mat4 projMatrix;
	glm::mat4 viewMatrix;

	json camAnimsJSON;
	std::vector<CameraAnimation> camAnims;
	std::vector<CameraAnimation>::iterator camAnimsIterator;
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
	void Update();
private:
	void ReadCamAnimsJSON();
};

