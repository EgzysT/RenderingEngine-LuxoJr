#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Graphics.h"
#include "InputManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

class Application
{
public:
	static Application* GetInstance();
private:
	static std::unique_ptr<Application> instance;
	GLFWwindow* window;
	InputManager inputManager;
	std::unique_ptr<Graphics> graphics;
	int width;
	int height;
public:
	Application() {};
	int Init();		// Initialization
	int DebugTemp();	// Debug or Temporary Code	
	int Run();			// Main Loop
	void Terminate();	// Terminate Execution
};