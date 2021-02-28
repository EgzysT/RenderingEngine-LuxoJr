#include "RenderItem.h"

#include <limits>

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
	//rotationAxis = glm::sphericalRand(1.0);

	float speed = 20.0f;
	//rotationSpeed = glm::gaussRand(speed, 3.0f);

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
	//mesh->Render(boundingBoxVBO.get());
	mesh->Render();
}

void RenderItem::RenderBoundingBox()
{
	glm::mat4 mvpMatrix = graphics->camera.getProjMatrix() * graphics->camera.getViewMatrix() * modelMatrix;
	graphics->boxShader->SetUniformMatrix4("uMVPMatrix", mvpMatrix);
	aabb.Render(boundingBoxVBO.get());
}

void RenderItem::Update(double deltaTime)
{
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationSpeed * (float)deltaTime), rotationAxis);
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

void RenderItem::CalculateBoundingBox()
{
	float min_x, min_y, min_z, max_x, max_y, max_z;
	//min_x = std::numeric_limits<float>::max();
	//min_y = std::numeric_limits<float>::max();
	//min_z = std::numeric_limits<float>::max();
	//max_x = std::numeric_limits<float>::min();
	//max_y = std::numeric_limits<float>::min();
	//max_z = std::numeric_limits<float>::min();
	min_x = 10000.0f;
	min_y = 10000.0f;
	min_z = 10000.0f;
	max_x = -10000.0f;
	max_y = -10000.0f;
	max_z = -10000.0f;

	for (int i = 0; i < mesh->vertices.size(); i++) {
		glm::vec3 vert = modelMatrix * glm::vec4(mesh->vertices.at(i).pos, 1.0);
		//glm::vec3 vert = glm::vec4(mesh->vertices.at(i).pos, 1.0);
		// x-axis
		if (vert.x < min_x)
			min_x = vert.x;
		if (vert.x > max_x)
			max_x = vert.x;
		// y-axis
		if (vert.y < min_y)
			min_y = vert.y;
		if (vert.y > max_y)
			max_y = vert.y;
		// z-axis
		if (vert.z < min_z)
			min_z = vert.z;
		if (vert.z > max_z)
			max_z = vert.z;
	}
	boundingMin = glm::vec3(min_x, min_y, min_z);
	boundingMax = glm::vec3(max_x, max_y, max_z);
	aabb = BoundingBox(boundingMin, boundingMax);
	glm::vec3 boundingMin2 = glm::vec3(glm::inverse(modelMatrix) * glm::vec4(boundingMin, 1.0));
	glm::vec3 boundingMax2 = glm::vec3(glm::inverse(modelMatrix) * glm::vec4(boundingMax, 1.0));
	boundingMin = glm::inverse(modelMatrix) * glm::vec4(boundingMin, 1.0);
	boundingMax = glm::inverse(modelMatrix) * glm::vec4(boundingMax, 1.0);

	std::vector<float> verts = {
		//front
		boundingMin.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		//back
		boundingMin.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		//top
		boundingMin.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMax.y, boundingMax.z, 1.0f,0.5f,0.0f,
		//bottom
		boundingMin.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMin.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMin.z, 1.0f,0.5f,0.0f,
		boundingMax.x, boundingMin.y, boundingMax.z, 1.0f,0.5f,0.0f

		////front
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		////back
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		////top
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMin.z,
		//boundingMin.x, boundingMax.y, boundingMax.z,
		//boundingMax.x, boundingMax.y, boundingMin.z,
		//boundingMax.x, boundingMax.y, boundingMax.z,
		////bottom
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMax.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMin.z,
		//boundingMin.x, boundingMin.y, boundingMax.z,
		//boundingMax.x, boundingMin.y, boundingMin.z,
		//boundingMax.x, boundingMin.y, boundingMax.z
	};

	boundingBoxVBO = std::make_shared<VertexBuffer>(&verts[0], sizeof(float) * verts.size());
}