#include "Time.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

std::shared_ptr<Time> Time::instance;

Time* Time::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<Time>();
    }
    return instance.get();
}

void Time::BeginTime()
{
    frameCount = 0;
    totalFrameCount = 0;
    deltaTime = 0.0f;
    lastTime = glfwGetTime();
    beginTime = lastTime;
    loadingFinishedTime = lastTime;
    std::cout << "Loading time: " << loadingFinishedTime << " s" << std::endl;
}

void Time::NewFrameTime()
{
    double time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime = time;
    frameCount++;
    totalFrameCount++;
    if (time - beginTime >= 0.5) {
        double avgFrameTime = (time - beginTime) / (double)frameCount;
        double avgFPS = 1.0 / avgFrameTime;
        std::cout << "Avrg Frame: " << avgFrameTime*1000 << " ms    FPS: " << avgFPS << std::endl;
        frameCount = 0;
        beginTime = time;
    }
}

double Time::GetCurrentLifetime()
{
    return glfwGetTime() - loadingFinishedTime;
}

Time::Time()
    : loadingFinishedTime(0.0), totalFrameCount(0)
{
}

Time::~Time()
{
    double avgFrameTime = (lastTime - loadingFinishedTime) / (double)totalFrameCount;
    double avgFPS = 1.0 / avgFrameTime;
    std::cout << "End - Avrg Frame: " << avgFrameTime * 1000 << " ms    FPS: " << avgFPS << std::endl;
}

