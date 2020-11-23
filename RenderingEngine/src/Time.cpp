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
    deltaTime = 0.0f;
    lastTime = glfwGetTime();
    beginTime = lastTime;
}

void Time::NewFrameTime()
{
    double time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime = time;
    frameCount++;
    if (time - beginTime >= 0.5) {
        std::cout << (double)frameCount / (time - beginTime) << std::endl;
        frameCount = 0;
        beginTime = time;
    }
}
