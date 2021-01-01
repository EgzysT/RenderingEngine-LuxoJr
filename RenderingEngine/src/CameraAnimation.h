#pragma once

#include <GLM/glm.hpp>

//#include "Camera.h"

class Camera;

class CameraAnimation
{
public:
	Camera* camera;
	double startTime;
	double endTime;
	glm::vec3 eyeInit;
	glm::vec3 eyeFinal;
	glm::vec3 centerInit;
	glm::vec3 centerFinal;

	CameraAnimation(Camera* camera, double startTime, double endTime, 
		glm::vec3 eyeInit, glm::vec3 eyeFinal, glm::vec3 centerInit, glm::vec3 centerFinal);
	void Update();
};

