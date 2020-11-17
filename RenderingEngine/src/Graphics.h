#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "RenderItem.h"

class RenderItem;

struct application_window {
	int width = 0;
	int height = 0;
	Camera* camera = nullptr;
	bool mouse_left = false;
	bool mouse_right = false;
	double lastx = 0.0;
	double lasty = 0.0;
};

class Graphics
{
public:
	GLFWwindow* window;
	application_window app_window;
	Camera camera;
	std::shared_ptr<Shader> activeShader;
private:
	std::vector<Light> lights;
	std::vector<RenderItem> renderItems;

public:
	Graphics(GLFWwindow* window, int width, int height);
	~Graphics();
	void Render();
private:
	void InitShader();
	void InitLights();
	void LoadMeshes();
	void UpdateCamera();
	void UpdateLights();
	void DisplayItems();

	void DebugFunction();
};

