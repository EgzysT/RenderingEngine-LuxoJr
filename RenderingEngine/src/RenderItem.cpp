#include "RenderItem.h"

#include <GLM/gtc/matrix_inverse.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/random.hpp>

#include "AssetManager.h"
#include "Utils.h"

RenderItem::RenderItem(Graphics* graphics, std::string meshString)
	: graphics(graphics)
{
	//modelMatrix = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.2));
	modelMatrix = glm::mat4(1.0f);
	rotationAxis = glm::sphericalRand(1.0);

	float speed = 20.0f;
	rotationSpeed = glm::gaussRand(speed, 3.0f);

	mesh = AssetManager::GetInstance()->GetMesh(meshString);
}

void RenderItem::Render(bool isShadowPass)
{
	if (isShadowPass) {
		graphics->depthShader->SetUniformMatrix4("model", modelMatrix);
	}
	else {
		glm::mat4 modelViewMatrix = graphics->camera.getViewMatrix() * modelMatrix;
		glm::mat4 normalMatrix = glm::inverseTranspose(modelViewMatrix); // similar to transpose(inverse())
		graphics->activeShader->SetUniformMatrix4("u_ModelMatrix", modelMatrix);
		graphics->activeShader->SetUniformMatrix4("u_ViewMatrix", graphics->camera.getViewMatrix());
		graphics->activeShader->SetUniformMatrix4("u_NormalMatrix", normalMatrix);
	}
	mesh->Render();
}

void RenderItem::Update(double deltaTime)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationSpeed * (float)deltaTime), rotationAxis);
}

void RenderItem::SetPosition(double x, double y, double z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void RenderItem::SetRotation(float x, float y, float z)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(z), glm::vec3(0, 0, 1));
}

void RenderItem::SetScale(double x, double y, double z)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
}
