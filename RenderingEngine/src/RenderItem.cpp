#include "RenderItem.h"

#include <GLM/gtc/matrix_inverse.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Utils.h"

RenderItem::RenderItem(Graphics* graphics, Mesh mesh) 
	: graphics(graphics), mesh(mesh)
{
	//modelMatrix = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.2));
	modelMatrix = glm::mat4(1.0f);
}

void RenderItem::Render()
{
	glm::mat4 modelViewMatrix = graphics->camera.getViewMatrix() * modelMatrix;
	glm::mat4 normalMatrix = glm::inverseTranspose(modelViewMatrix); // similar to transpose(inverse())
	graphics->activeShader->SetUniformMatrix4("u_ModelViewMatrix", modelViewMatrix);
	graphics->activeShader->SetUniformMatrix4("u_NormalMatrix", normalMatrix);
	mesh.Render();
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
