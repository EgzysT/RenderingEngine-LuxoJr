#include "CameraAnimation.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Time.h"

CameraAnimation::CameraAnimation(Camera* camera, double startTime, double endTime,
	glm::vec3 eyeInit, glm::vec3 eyeFinal, glm::vec3 centerInit, glm::vec3 centerFinal)
	: camera(camera), startTime(startTime), endTime(endTime), eyeInit(eyeInit), eyeFinal(eyeFinal), centerInit(centerInit), centerFinal(centerFinal)
{
    
}

void CameraAnimation::Update()
{
	double dTime = Time::GetInstance()->deltaTime;

	double time = glfwGetTime();
	double a = (time - startTime) / (endTime - startTime);

	camera->eye = glm::mix(eyeInit, eyeFinal, a);
	camera->center = glm::mix(centerInit, centerFinal, a);
	camera->dirty = true;
}
