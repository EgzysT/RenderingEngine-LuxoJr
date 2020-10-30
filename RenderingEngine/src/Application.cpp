#include "Application.h"

#include <stb_image.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"

std::unique_ptr<Application> Application::instance;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

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
    /* Initialize the library */
    if (!glfwInit())
        ERROR_EXIT("Failed to initialize GLFW");

    //#if _DEBUG
    //    // Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //#endif
    //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);                     // Fixed size window

    width = 1280;
    height = 768;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Real-time Rendering 2020", nullptr, nullptr);
    if (!window)
    {
        ERROR_EXIT("Failed to create window or context");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //TODO: This activates/deactivates vsync
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
    glfwSetKeyCallback(window, key_callback);

    return 0;
}

int Application::DebugTemp()
{
    mesh = Mesh{};
    mesh.LoadMesh("..\\assets\\models\\barrel\\barrel.fbx");
    //mesh.LoadMesh("..\\assets\\models\\TrashCan\\trashcan.fbx");
    //mesh.LoadMesh("..\\assets\\models\\rocks\\RockSet.obj");

    float vertices[] = {
        //positions          //texture coords
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    activeVB = std::make_unique<VertexBuffer>(vertices, 4 * 5 * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    activeIB = std::make_unique<IndexBuffer>(indices, 6);

    //TODO: The paths are relative to the project root when running inside VS, might not work in standalone
    activeShader = std::make_unique<Shader>("..\\assets\\shaders\\vert.glsl", "..\\assets\\shaders\\frag.glsl");
    activeShader->Bind();

    activeTexture = std::make_unique<Texture>("..\\assets\\textures\\test.png");
    activeTexture->Bind();
    activeShader->SetUniformInteger("u_Texture", 0);

    camera = std::make_unique<Camera>(90, width, height, 0.01, 1000.0);
    //glm::mat4 model = glm::mat4(1.0);
    //glm::mat4 model = RotMat4(15.0, 30.0, 5.0);
    glm::mat4 model = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.9));
    //glm::mat4 mvpMatrix = proj * view * model;
    glm::mat4 mvpMatrix = camera->getViewProjMatrix() * model;

    activeShader->SetUniformMatrix4("u_MVP", mvpMatrix);

    r = 0.0f;
    increment = 0.035f;

    return 0;
}

int Application::Run()
{
    unsigned int frameCount = 0;
    unsigned long long beginClock;
    unsigned long long endClock;
    glEnable(GL_DEPTH_TEST);

    beginClock = GetTickCount64();
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //activeShader->SetUniformVec4("u_Color", r, 0.3f, 0.8f, 1.0f);

        activeShader->Bind();
        activeVB->Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        activeIB->Bind();
        activeTexture->Bind();
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (r < 0.0f) {
            increment = 0.035f;
        }
        else if (r > 1.0f) {
            increment = -0.035f;
        }
        r += increment;

        mesh.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        endClock = GetTickCount64();
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
