#include "Application.h"

#include <stb_image.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtc/matrix_inverse.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#include "Light.h"

std::unique_ptr<Application> Application::instance;

Application* Application::GetInstance()
{
    if (!instance)
    {
        instance = std::make_unique<Application>();
    }
    return instance.get();
}

int Application::Init()
{
    glfwSetErrorCallback(glfwDebugCallback);

    /* Initialize the library */
    if (!glfwInit())
        ERROR_EXIT("Failed to initialize GLFW");

    #if _DEBUG
        // Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);                     // Fixed size window

    //width = 1280;
    //height = 768;
    width = 1600;
    height = 900;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Real-time Rendering 2020", nullptr, nullptr);
    if (!window)
    {
        ERROR_EXIT("Failed to create window or context");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //NOTE: This activates/deactivates vsync
    glfwSwapInterval(1);
    //glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // if glewInit fails, something is seriously wrong.
        ERROR_EXIT(glewGetErrorString(err));
    }

#if _DEBUG
    // Register your callback function.
    glDebugMessageCallback(DebugCallback, NULL);

    // Enable synchronous callback. This ensures that your callback function is called
    // right after an error has occurred. 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*
        INPUT
    */
    inputManager.InitInput(window);

    graphics = std::make_unique<Graphics>(window, width, height);

    return 0;
}

int Application::DebugTemp()
{
    return 0;
}

int Application::Run()
{
    unsigned int frameCount = 0;
    unsigned long long beginClock;
    unsigned long long endClock;

    double deltaTime = 0.0f;
    double lastTime = 0.0f;
    beginClock = GetTickCount64();
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        graphics->Render();
   
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        endClock = GetTickCount64();
        double time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;
        frameCount++;
        //std::cout << (endClock - beginClock) << std::endl;
        if (endClock - beginClock >= 1000) {
            std::cout << (double)frameCount / ((endClock - beginClock) * 0.001) << std::endl;
            frameCount = 0;
            beginClock = endClock;
        }
    }

    return 0;
}

void Application::Terminate()
{
    glfwTerminate();
}
