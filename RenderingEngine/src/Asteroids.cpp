#include "Asteroids.h"
#include "Graphics.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_inverse.hpp>
#include <GLM/gtc/matrix_transform.hpp>

Asteroids::Asteroids(std::vector<RenderItem>* renderItems)
	: renderItems(renderItems)
{
}

Asteroids::~Asteroids()
{
    delete(modelMatrices);
}

void Asteroids::CreateModelMatrices()
{
    unsigned int amount = renderItems->size();
    modelMatrices = new glm::mat4[amount];
	for (int i = 0; i < amount; i++)
	{
		modelMatrices[i] = renderItems->at(i).modelMatrix;
	}
}

void Asteroids::ConfigureInstancedArray()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, renderItems->size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    Mesh* asteroid = renderItems->at(0).mesh.get();

	for (int i = 0; i < asteroid->meshEntries.size(); i++)
	{
        unsigned int VAO = asteroid->meshEntries[i].vao;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);

        glBindVertexArray(0);
	}
}

void Asteroids::Render()
{

    for (int i = 0; i < renderItems->size(); i++)
    {
        renderItems->at(i).Render(false);
    }
}

void Asteroids::RenderInstanced()
{
    Graphics* graphics = renderItems->at(0).graphics;
    Shader* instancesShader = graphics->instancesShader.get();

    instancesShader->SetUniformMatrix4("u_ViewMatrix", renderItems->at(0).graphics->camera.getViewMatrix());

    const unsigned int MaterialIndex = renderItems->at(0).mesh->meshEntries[0].matIndex;
    if (MaterialIndex < renderItems->at(0).mesh->renderMaterials.size() && renderItems->at(0).mesh->renderMaterials[MaterialIndex]) {
        renderItems->at(0).mesh->renderMaterials[MaterialIndex]->Bind();
    }

    Mesh* asteroid = renderItems->at(0).mesh.get();
    for (int i = 0; i < asteroid->meshEntries.size(); i++)
    {
        glBindVertexArray(asteroid->meshEntries[i].vao);

        //glm::mat4 modelViewMatrix = graphics->camera.getViewMatrix() * modelMatrices[i];
        //glm::mat4 normalMatrix = glm::inverseTranspose(modelViewMatrix); // similar to transpose(inverse())
        //instancesShader->SetUniformMatrix4("u_NormalMatrix", normalMatrix);

        glDrawElementsInstanced(GL_TRIANGLES, asteroid->meshEntries[i].numIndices, GL_UNSIGNED_INT, 0, renderItems->size());
        //glBindVertexArray(0);
    }
    glBindVertexArray(0);
}
