#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

//#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "RenderItem.h"
#include "Skybox.h"

class RenderItem;
class Application;

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
	Application* app;
	GLFWwindow* window;
	application_window app_window;
	Camera camera;
	std::shared_ptr<Shader> activeShader;
	std::shared_ptr<Shader> skyboxShader;
	std::shared_ptr<Shader> depthShader;
	std::shared_ptr<Shader> depthShaderDebug;
private:
	std::vector<Light> lights;
	std::vector<RenderItem> renderItems;
	std::shared_ptr<Skybox> skybox;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	unsigned int srcWidth;
	unsigned int srcHeight;
	const unsigned int SHADOW_WIDTH = 2048;
	const unsigned int SHADOW_HEIGHT = 2048;
	unsigned int quadVBO = 0;
public:
	Graphics(Application* app, GLFWwindow* window, int width, int height);
	~Graphics();
	void Render();
	void Update(double deltaTime);
	void UpdateCamera();
private:
	void InitShader();
	void InitLights();
	void LoadScene();
	void LoadSkybox();
	void UpdateLights();
	void DisplayItems(bool isShadowPass);

	void CreateDepthMap();
	void RenderDepthQuad();
};

