#pragma once

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "RenderItem.h"
class RenderItem;
class Graphics
{
public:
	Camera camera;
	std::shared_ptr<Shader> activeShader;
private:
	std::vector<Light> lights;
	std::vector<RenderItem> renderItems;

public:
	Graphics(Camera camera, std::shared_ptr<Shader> activeShader);
	~Graphics();
	void Display();
private:
	void InitLights();
	void LoadMeshes();
	
};

