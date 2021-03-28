#pragma once

#include <vector>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"

class Skybox
{
private:
	unsigned int cubemapTexture;
	unsigned int skyboxVAO;
	std::shared_ptr<VertexBuffer> skyboxVBO;

public:
	Skybox(std::vector<std::string> faces);
	~Skybox();
	void Render(std::shared_ptr<Shader> shader, Camera* camera);
private:
	unsigned int LoadCubemap(std::vector<std::string> faces);
};

