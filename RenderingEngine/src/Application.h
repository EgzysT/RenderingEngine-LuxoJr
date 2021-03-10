#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Graphics.h"
//#include "InputManager.h"

//class Graphics;
class InputManager;

class Application
{
public:
	static Application* GetInstance();
	int width;
	int height;
private:
	static std::unique_ptr<Application> instance;
	GLFWwindow* window;
	InputManager* inputManager;
	std::unique_ptr<Graphics> graphics;
public:
	Application() {};
	int Init();			// Initialization
	//int DebugTemp();	// Debug or Temporary Code	
	int Run();			// Main Loop
	void Terminate();	// Terminate Execution
};