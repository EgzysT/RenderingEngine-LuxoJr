#include "Graphics.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_inverse.hpp>

#include <iostream>

#include "Utils.h"

Graphics::Graphics(GLFWwindow* window, int width, int height)
    : camera(glm::radians(90.0), (double)width / height, 0.01, 1000.0)
{
    app_window = application_window{ width, height, &camera };
    glfwSetWindowUserPointer(window, &app_window);

    // VAO (needed for core profile)
    unsigned int vertexArrayObject;    //TODO: Make this useful?
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    InitShader();

    InitLights();
    LoadMeshes();

    DebugFunction();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UpdateLights();

    DisplayItems();
}

void Graphics::InitShader()
{
    //TODO: The paths are relative to the project root when running inside VS, might not work in standalone
    activeShader = std::make_shared<Shader>("..\\assets\\shaders\\vert.glsl", "..\\assets\\shaders\\frag.glsl");
    activeShader->Bind();
    activeShader->SetUniformInteger("u_Texture", 0);
    activeShader->SetUniformFloat("u_matSpecular", 0.9);
    activeShader->SetUniformFloat("u_matShininess", 15);

    activeShader->SetUniformMatrix4("u_ProjMatrix", camera.getProjMatrix());
}

void Graphics::InitLights()
{
    //DirectionalLight light1 = DirectionalLight(0);
    //light1.SetDirection(1.0, 0.0, 0.0);
    //light1.SetAmbient(0.02, 0.02, 0.02);
    ////light1.SetAmbient(0.2, 0.95, 0.91);
    //light1.SetDiffuse(0.2, 0.2, 0.2);
    //light1.SetSpecular(0.2, 0.2, 0.2);
    //light1.SetAttenuations(1.0, 0.14, 0.07);
    //lights.push_back(light1);

    //DirectionalLight light2 = DirectionalLight(1);
    //light2.SetDirection(-0.2, -0.2, -0.8);
    //light2.SetAmbient(0.02, 0.02, 0.02); 
    ////light2.SetAmbient(0.2, 0.95, 0.91);
    //light2.SetDiffuse(1.0, 1.0, 1.0);
    //light2.SetSpecular(1.0, 1.0, 1.0);
    ////light2.SetSpecular(0.0, 0.0, 0.0);
    //light2.SetAttenuations(1.0, 0.14, 0.07);
    //lights.push_back(light2);

    PointLight light1 = PointLight(0);
    light1.SetPosition(3.8, 0.0, 0.0);
    light1.SetAmbient(0.02, 0.02, 0.02);
    //light1.SetAmbient(0.2, 0.95, 0.91);
    //light1.SetDiffuse(0.2, 0.2, 0.2);
    light1.SetDiffuse(1.0, 1.0, 1.0);
    light1.SetSpecular(0.0, 0.0, 0.0);
    light1.SetAttenuations(1.0, 0.14, 0.07);
    lights.push_back(light1);

    PointLight light2 = PointLight(1);
    light2.SetPosition(-2.0, -1.0, -2.5);
    light2.SetAmbient(0.02, 0.02, 0.02);
    //light2.SetAmbient(0.2, 0.95, 0.91);
    light2.SetDiffuse(1.0, 1.0, 1.0);
    light2.SetSpecular(1.0, 1.0, 1.0);
    //light2.SetSpecular(0.0, 0.0, 0.0);
    light2.SetAttenuations(1.0, 0.14, 0.07);
    lights.push_back(light2);
}

void Graphics::LoadMeshes()
{
    Mesh mesh = Mesh{};
    mesh.LoadMesh("..\\assets\\models\\barrel\\barrel.fbx");
    //mesh.LoadMesh("..\\assets\\models\\TrashCan\\trashcan.fbx");
    //mesh.LoadMesh("..\\assets\\models\\rocks\\RockSet.obj");    
    RenderItem renderItem{ this, mesh };
    renderItems.push_back(renderItem);
}

void Graphics::UpdateCamera()
{

}

void Graphics::UpdateLights()
{
    glm::mat4 view = camera.getViewMatrix();
    for (size_t i = 0; i < lights.size(); i++)
    {
        lights[i].UpdateShader(activeShader, view);
    }
}

void Graphics::DisplayItems()
{
    for (size_t i = 0; i < renderItems.size(); i++)
    {
        renderItems[i].Render();
    }
}

void Graphics::DebugFunction()
{

}
