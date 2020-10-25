#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Application
{
public:
	static Application* GetInstance();
private:
	static std::unique_ptr<Application> instance;
	//std::unique_ptr<GLFWwindow> window;
	GLFWwindow* window;
	std::unique_ptr<Shader> activeShader;
	std::unique_ptr<VertexBuffer> activeVB;
	std::unique_ptr<IndexBuffer> activeIB;
	std::unique_ptr<Texture> activeTexture;
	int width;
	int height;
	float r;
	float increment;
public:
	Application() {}
	int Init();		// Initialization
	int DebugTemp();	// Debug or Temporary Code	
	int Run();			// Main Loop
	void Terminate();	// Terminate Execution
};