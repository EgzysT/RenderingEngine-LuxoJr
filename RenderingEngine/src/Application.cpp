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

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto win = (application_window*)glfwGetWindowUserPointer(window);

    if (win->mouse_left) {
        auto xoffset = ((double)xpos - win->lastx) / win->width;
        auto yoffset = ((double)ypos - win->lasty) / win->height;
        win->lastx = xpos;
        win->lasty = ypos;

        win->camera->rotate(xoffset, yoffset);
    }
    if (win->mouse_right) {
        auto xoffset = ((double)xpos - win->lastx) / win->width;
        auto yoffset = ((double)ypos - win->lasty) / win->height;
        win->lastx = xpos;
        win->lasty = ypos;

        win->camera->pan(xoffset, yoffset);
    }
}


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto win = (application_window*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        win->mouse_left = true;
        glfwGetCursorPos(window, &win->lastx, &win->lasty);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        win->mouse_left = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        win->mouse_right = true;
        glfwGetCursorPos(window, &win->lastx, &win->lasty);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        win->mouse_right = false;
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto win = (application_window*)glfwGetWindowUserPointer(window);
    win->camera->dolly(yoffset / 10);
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
    glfwSetErrorCallback(error_callback);

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    app_window = application_window{width, height};
    glfwSetWindowUserPointer(window, &app_window);

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
    activeShader = std::make_shared<Shader>("..\\assets\\shaders\\vert.glsl", "..\\assets\\shaders\\frag.glsl");
    activeShader->Bind();

    activeTexture = std::make_unique<Texture>("..\\assets\\textures\\test.png");
    activeTexture->Bind();
    activeShader->SetUniformInteger("u_Texture", 0);

    camera = std::make_unique<Camera>(glm::radians(90.0), (double)width / height, 0.01, 1000.0);
    app_window.camera = camera.get();
    //glm::mat4 model = glm::mat4(1.0);
    //glm::mat4 model = RotMat4(15.0, 30.0, 5.0);
    glm::mat4 model = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.9));
    //glm::mat4 mvpMatrix = proj * view * model;
    //glm::mat4 mvpMatrix = camera->getViewProjMatrix() * model;

    activeShader->SetUniformMatrix4("u_modelMatrix", model);

    activeShader->SetUniformFloat("u_matSpecular", 0.9);
    activeShader->SetUniformFloat("u_matShininess", 15);

    r = 0.0f;
    increment = 0.035f;

    return 0;
}

int Application::Run()
{
    //DirectionalLight light1 = DirectionalLight(0);
    //light1.SetDirection(1.0, 0.0, 0.0);
    //light1.SetAmbient(0.02, 0.02, 0.02);
    ////light1.SetAmbient(0.2, 0.95, 0.91);
    //light1.SetDiffuse(0.2, 0.2, 0.2);
    //light1.SetSpecular(0.2, 0.2, 0.2);
    //light1.SetAttenuations(1.0, 0.14, 0.07);
    //light1.UpdateShader(activeShader);

    //DirectionalLight light2 = DirectionalLight(1);
    //light2.SetDirection(-0.2, -0.2, -0.8);
    //light2.SetAmbient(0.02, 0.02, 0.02); 
    ////light2.SetAmbient(0.2, 0.95, 0.91);
    //light2.SetDiffuse(1.0, 1.0, 1.0);
    //light2.SetSpecular(1.0, 1.0, 1.0);
    ////light2.SetSpecular(0.0, 0.0, 0.0);
    //light2.SetAttenuations(1.0, 0.14, 0.07);
    //light2.UpdateShader(activeShader);

    PointLight light1 = PointLight(0);
    light1.SetPosition(3.8, 0.0, 0.0);
    light1.SetAmbient(0.02, 0.02, 0.02);
    //light1.SetAmbient(0.2, 0.95, 0.91);
    //light1.SetDiffuse(0.2, 0.2, 0.2);
    light1.SetDiffuse(1.0, 1.0, 1.0);
    light1.SetSpecular(0.0, 0.0, 0.0);
    light1.SetAttenuations(1.0, 0.14, 0.07);
    //light1.UpdateShader(activeShader);

    PointLight light2 = PointLight(1);
    light2.SetPosition(-2.4, -1.4, -3.0);
    light2.SetAmbient(0.02, 0.02, 0.02);
    //light2.SetAmbient(0.2, 0.95, 0.91);
    light2.SetDiffuse(1.0, 1.0, 1.0);
    light2.SetSpecular(1.0, 1.0, 1.0);
    //light2.SetSpecular(0.0, 0.0, 0.0);
    light2.SetAttenuations(1.0, 0.14, 0.07);
    //light2.UpdateShader(activeShader);

    unsigned int frameCount = 0;
    unsigned long long beginClock;
    unsigned long long endClock;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double deltaTime = 0.0f;
    double lastTime = 0.0f;
    beginClock = GetTickCount64();
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //activeShader->SetUniformMatrix4("u_ViewProjMatrix", camera->getViewProjMatrix());
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 model = glm::scale(std::move(RotMat4(30.0, 50.0, 10.0)), glm::vec3(0.9));
        glm::mat4 modelViewMatrix = view * model;
        glm::mat4 normalMatrix = glm::inverseTranspose(view * model); // similar to transpose(inverse())
        activeShader->SetUniformMatrix4("u_ModelViewMatrix", modelViewMatrix);
        activeShader->SetUniformMatrix4("u_ProjMatrix", camera->getProjMatrix());
        activeShader->SetUniformMatrix4("u_NormalMatrix", normalMatrix);
        light1.UpdateShader(activeShader, view);
        light2.UpdateShader(activeShader, view);
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
