#include "CameraAnimation.h"

#include "Time.h"

CameraAnimation::CameraAnimation(Camera& camera, double startTime, double duration, glm::vec3 initPos, glm::vec3 finalPos, glm::vec3 initRot, glm::vec3 finalRot)
	: camera(camera), startTime(startTime), duration(duration), initPos(initPos), finalPos(finalPos), initRot(initRot), finalRot(finalRot)
{

}

void CameraAnimation::Update()
{
	double dTime = Time::GetInstance()->deltaTime;
}
