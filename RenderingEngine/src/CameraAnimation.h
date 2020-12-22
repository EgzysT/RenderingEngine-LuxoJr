#pragma once

#include <GLM/glm.hpp>

#include "Camera.h"

class CameraAnimation
{
private:
	Camera& camera;
	double startTime;
	double duration;
	glm::vec3 initPos;
	glm::vec3 finalPos;
	glm::vec3 initRot;
	glm::vec3 finalRot;
public:
	CameraAnimation(Camera& camera, double startTime, double duration, 
		glm::vec3 initPos, glm::vec3 finalPos, glm::vec3 initRot, glm::vec3 finalRot);
	void Update();
};

