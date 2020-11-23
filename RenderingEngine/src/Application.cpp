#include "Application.h"

#include <iostream>

#include "Time.h"
#include "Utils.h"

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

    inputManager = InputManager::GetInstance();
    inputManager->InitInput(window);

    graphics = std::make_unique<Graphics>(this, window, width, height);

    return 0;
}

//int Application::DebugTemp()
//{
//    return 0;
//}

int Application::Run()
{
    Time* time = Time::GetInstance();
    time->BeginTime();
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        graphics->Render();
   
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        inputManager->ProcessInput();
        
        time->NewFrameTime();
    }
    return 0;
}

void Application::Terminate()
{
    glfwTerminate();
}
