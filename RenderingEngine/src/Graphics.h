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
#include "Octree.h"
#include "application_window.h"

class RenderItem;
class Application;

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
	std::shared_ptr<Shader> boxShader;
	std::shared_ptr<Node> octreeRoot;
	bool runCullOctree;
	bool runDisplayOctree;
	bool runDisplayBoundBoxes;
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
	void DisplayItemsOctree();
	void DisplayAABB();
	void CreateOctree();

	void CreateDepthMap();
	void RenderDepthQuad();
};

