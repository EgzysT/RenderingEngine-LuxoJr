#include "RenderItem.h"

#include <GLM/gtc/matrix_inverse.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Utils.h"

RenderItem::RenderItem(Graphics* graphics, Mesh mesh) 
	: graphics(graphics), mesh(mesh)
{
	modelMatrix = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.9));
}

void RenderItem::Render()
{
	glm::mat4 modelViewMatrix = graphics->camera.getViewMatrix() * modelMatrix;
	glm::mat4 normalMatrix = glm::inverseTranspose(modelViewMatrix); // similar to transpose(inverse())
	graphics->activeShader->SetUniformMatrix4("u_ModelViewMatrix", modelViewMatrix);
	graphics->activeShader->SetUniformMatrix4("u_NormalMatrix", normalMatrix);
	mesh.Render();
}
