#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

struct application_window {
	int width = 0;
	int height = 0;
	Camera* camera = nullptr;
	bool mouse_left = false;
	bool mouse_right = false;
	double lastx = 0.0;
	double lasty = 0.0;
};

class Application
{
public:
	static Application* GetInstance();
private:
	static std::unique_ptr<Application> instance;
	//std::unique_ptr<GLFWwindow> window;
	GLFWwindow* window;
	application_window app_window;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Shader> activeShader;
	std::unique_ptr<VertexBuffer> activeVB;
	std::unique_ptr<IndexBuffer> activeIB;
	std::unique_ptr<Texture> activeTexture;
	int width;
	int height;
	float r;
	float increment;
	Mesh mesh;
public:
	Application() {}
	int Init();		// Initialization
	int DebugTemp();	// Debug or Temporary Code	
	int Run();			// Main Loop
	void Terminate();	// Terminate Execution
};