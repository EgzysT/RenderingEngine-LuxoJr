#include "Graphics.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_inverse.hpp>

#include <iostream>

#include "Scene.h"
#include "Utils.h"

Graphics::Graphics(Application* app, GLFWwindow* window, int width, int height)
    : camera(glm::radians(90.0), (double)width / height, 0.01, 1000.0),
    app(app), srcWidth(width), srcHeight(height)
{
    app_window = application_window{ width, height, &camera };
    glfwSetWindowUserPointer(window, &app_window);

    // VAO (needed for core profile)
    unsigned int vertexArrayObject;    //TODO: Make this useful?
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    InitShader();

    InitLights();
    LoadScene();
    LoadSkybox();

    CreateDepthMap();

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

    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------

    //glm::vec3 lightPos(0.0, 0.0, 30.0);
    //glm::mat4 lightProjection, lightView;
    //glm::mat4 lightSpaceMatrix;
    //float near_plane = 1.0f, far_plane = 50.0f;
    //lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, near_plane, far_plane);
    //lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    //lightSpaceMatrix = lightProjection * lightView;
    //// render scene from light's point of view
    //depthShader->Bind();
    //depthShader->SetUniformMatrix4("lightSpaceMatrix", lightSpaceMatrix);

    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    ////glActiveTexture(GL_TEXTURE0);
    ////glBindTexture(GL_TEXTURE_2D, woodTexture);
    //glCullFace(GL_FRONT);
    //DisplayItems(true);
    //glCullFace(GL_BACK);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// reset viewport
    //glViewport(0, 0, srcWidth, srcHeight);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. render scene as normal using the generated depth/shadow map  
    // --------------------------------------------------------------
    glViewport(0, 0, srcWidth, srcHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activeShader->Bind();
    UpdateLights();

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    //activeShader->SetUniformMatrix4("u_LightSpaceMatrix", lightSpaceMatrix);
    DisplayItems(false);

    boxShader->Bind();
    DisplayAABB();

    // draw skybox as last
    skybox->Render(skyboxShader, &camera);

    // render Depth map to quad for visual debugging
    // ---------------------------------------------
    //depthShaderDebug->Bind();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, depthMap);
    //RenderDepthQuad();
}

void Graphics::Update(double deltaTime)
{
    UpdateCamera();
    for (size_t i = 0; i < renderItems.size(); i++)
    {
        renderItems[i].Update(deltaTime);
    }
}

void Graphics::InitShader()
{
    //TODO: The paths are relative to the project root when running inside VS, might not work in standalone
    skyboxShader = std::make_shared<Shader>("..\\assets\\shaders\\vertSkybox.glsl", "..\\assets\\shaders\\fragSkybox.glsl");
    skyboxShader->Bind();

    depthShader = std::make_shared<Shader>("..\\assets\\shaders\\vertDepth.glsl", "..\\assets\\shaders\\fragDepth.glsl");
    depthShader->Bind();

    depthShaderDebug = std::make_shared<Shader>("..\\assets\\shaders\\vertDepthDebug.glsl", "..\\assets\\shaders\\fragDepthDebug.glsl");
    depthShaderDebug->Bind();
    depthShaderDebug->SetUniformInteger("depthMap", 0);

    boxShader = std::make_shared<Shader>("..\\assets\\shaders\\vertBox.glsl", "..\\assets\\shaders\\fragBox.glsl");
    boxShader->Bind();

    activeShader = std::make_shared<Shader>("..\\assets\\shaders\\vert.glsl", "..\\assets\\shaders\\frag.glsl");
    activeShader->Bind();
    activeShader->SetUniformInteger("u_Texture", 0); 
    activeShader->SetUniformInteger("u_NormalMap", 1);
    activeShader->SetUniformInteger("u_NormalMap", 1);
    //activeShader->SetUniformInteger("u_ShadowMap", 2);
    activeShader->SetUniformFloat("u_matSpecular", 0.99);
    activeShader->SetUniformFloat("u_matShininess", 20);

    activeShader->SetUniformMatrix4("u_ProjMatrix", camera.getProjMatrix());

    glLineWidth(2);
}

void Graphics::InitLights()
{
    DirectionalLight lightDir = DirectionalLight(0);
    lightDir.SetDirection(0.0, 0.0, -1.0);
    lightDir.SetAmbient(0.1, 0.1, 0.1);
    //lightDir.SetAmbient(0.2, 0.95, 0.91);
    lightDir.SetDiffuse(0.8, 0.8, 0.8);
    lightDir.SetSpecular(0.99, 0.99, 0.99);
    lightDir.SetAttenuations(1.0, 0.14, 0.07);
    lights.push_back(lightDir);

    //DirectionalLight light2 = DirectionalLight(1);
    //light2.SetDirection(-0.2, -0.2, -0.8);
    //light2.SetAmbient(0.02, 0.02, 0.02); 
    ////light2.SetAmbient(0.2, 0.95, 0.91);
    //light2.SetDiffuse(1.0, 1.0, 1.0);
    //light2.SetSpecular(1.0, 1.0, 1.0);
    ////light2.SetSpecular(0.0, 0.0, 0.0);
    //light2.SetAttenuations(1.0, 0.14, 0.07);
    //lights.push_back(light2);

    //PointLight light1 = PointLight(1);
    //light1.SetPosition(19.8, 5.0, -10.0);
    ////light1.SetAmbient(0.02, 0.02, 0.02);
    //light1.SetAmbient(0.08, 0.08, 0.08);
    ////light1.SetDiffuse(0.2, 0.2, 0.2);
    //light1.SetDiffuse(1.0, 1.0, 1.0);
    //light1.SetSpecular(1.0, 1.0, 1.0);
    //light1.SetAttenuations(1.0, 0.07, 0.007);
    //lights.push_back(light1);

    //PointLight light2 = PointLight(2);
    //light2.SetPosition(-2.0, -1.0, -8.5);
    //light2.SetAmbient(0.02, 0.02, 0.02);
    ////light2.SetAmbient(0.2, 0.95, 0.91);
    //light2.SetDiffuse(1.0, 1.0, 1.0);
    //light2.SetSpecular(1.0, 1.0, 1.0);
    ////light2.SetSpecular(0.0, 0.0, 0.0);
    ////light2.SetAttenuations(1.0, 0.14, 0.07);
    //light1.SetAttenuations(1.0, 0.07, 0.007);
    //lights.push_back(light2);
}

void Graphics::LoadScene()
{
    //mesh.LoadMesh("..\\assets\\models\\barrel\\barrel.fbx");
    //mesh.LoadMesh("..\\assets\\models\\TrashCan\\trashcan.fbx");
    //mesh.LoadMesh("..\\assets\\models\\rocks\\RockSet.obj");
    //mesh.LoadMesh("..\\assets\\models\\rock1\\Rock_1.fbx");
    //mesh.LoadMesh("barrel");
    //mesh.LoadMesh("rockBrown");

    Scene scene("..\\assets\\scene.json");
    renderItems = std::move(scene.CreateRenderItems(this));
}

void Graphics::LoadSkybox() {
    std::vector<std::string> faces{
        /*"..\\assets\\textures\\skybox1\\px.png", 
        "..\\assets\\textures\\skybox1\\nx.png",
        "..\\assets\\textures\\skybox1\\py.png",
        "..\\assets\\textures\\skybox1\\ny.png",
        "..\\assets\\textures\\skybox1\\pz.png",
        "..\\assets\\textures\\skybox1\\nz.png"*/
        "..\\assets\\textures\\skybox2\\right.png",
        "..\\assets\\textures\\skybox2\\left.png",
        "..\\assets\\textures\\skybox2\\top.png",
        "..\\assets\\textures\\skybox2\\bottom.png",
        "..\\assets\\textures\\skybox2\\front.png",
        "..\\assets\\textures\\skybox2\\back.png"
    };
    
    skybox = std::make_shared<Skybox>(faces);
}

void Graphics::UpdateCamera()
{
    camera.Update();
}

void Graphics::UpdateLights()
{
    glm::mat4 view = camera.getViewMatrix();
    for (size_t i = 0; i < lights.size(); i++)
    {
        lights[i].UpdateShader(activeShader, view);
    }
}

void Graphics::DisplayItems(bool isShadowPass)
{
    for (size_t i = 0; i < renderItems.size(); i++)
    {
        renderItems[i].Render(isShadowPass);
    }
}

void Graphics::DisplayAABB() {
    for (size_t i = 0; i < renderItems.size(); i++)
    {
        renderItems[i].RenderBoundingBox();
    }
}

void Graphics::CreateDepthMap()
{
    // configure depth map FBO
    // -----------------------
    
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Create depth map quad
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };
    // setup plane VAO
    //glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    //glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
}

void Graphics::RenderDepthQuad()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //}
    //glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glBindVertexArray(0);
}
