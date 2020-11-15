#include "Graphics.h"

Graphics::Graphics(Camera camera, std::shared_ptr<Shader> activeShader)
    : camera(camera), activeShader(activeShader)
{
}

void Graphics::Display()
{
	//UpdateCamera();
	//UpdateLights();
	//DisplayObjects();
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
    //light1.UpdateShader(activeShader);
    //lights.push_back(light1);

    //DirectionalLight light2 = DirectionalLight(1);
    //light2.SetDirection(-0.2, -0.2, -0.8);
    //light2.SetAmbient(0.02, 0.02, 0.02); 
    ////light2.SetAmbient(0.2, 0.95, 0.91);
    //light2.SetDiffuse(1.0, 1.0, 1.0);
    //light2.SetSpecular(1.0, 1.0, 1.0);
    ////light2.SetSpecular(0.0, 0.0, 0.0);
    //light2.SetAttenuations(1.0, 0.14, 0.07);
    //light2.UpdateShader(activeShader);
    //lights.push_back(light2);

    PointLight light1 = PointLight(0);
    light1.SetPosition(3.8, 0.0, 0.0);
    light1.SetAmbient(0.02, 0.02, 0.02);
    //light1.SetAmbient(0.2, 0.95, 0.91);
    //light1.SetDiffuse(0.2, 0.2, 0.2);
    light1.SetDiffuse(1.0, 1.0, 1.0);
    light1.SetSpecular(0.0, 0.0, 0.0);
    light1.SetAttenuations(1.0, 0.14, 0.07);
    //light1.UpdateShader(activeShader);
    lights.push_back(light1);

    PointLight light2 = PointLight(1);
    light2.SetPosition(-2.4, -1.4, -3.0);
    light2.SetAmbient(0.02, 0.02, 0.02);
    //light2.SetAmbient(0.2, 0.95, 0.91);
    light2.SetDiffuse(1.0, 1.0, 1.0);
    light2.SetSpecular(1.0, 1.0, 1.0);
    //light2.SetSpecular(0.0, 0.0, 0.0);
    light2.SetAttenuations(1.0, 0.14, 0.07);
    //light2.UpdateShader(activeShader);
    lights.push_back(light2);
}

void Graphics::LoadMeshes()
{
}
