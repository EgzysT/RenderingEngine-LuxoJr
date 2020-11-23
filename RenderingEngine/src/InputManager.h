#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Graphics.h"

class Application;
class InputManager
{
public:
    static InputManager* GetInstance();
private:
    static std::shared_ptr<InputManager> instance;
    GLFWwindow* window;
public:
    InputManager() {
        window = nullptr;
        wPressed = false;
        sPressed = false;
        aPressed = false;
        dPressed = false;
        ctrlPressed = false;
        shiftPressed = false;
    };
	void InitInput(GLFWwindow* window);
    void ProcessInput();

    bool wPressed;
    bool sPressed;
    bool aPressed;
    bool dPressed;
    bool ctrlPressed;
    bool shiftPressed;
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    InputManager* inputManager = InputManager::GetInstance();
    switch (key)
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS) inputManager->wPressed = true;
        else if (action == GLFW_RELEASE) {
            inputManager->wPressed = false;
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS) inputManager->sPressed = true;
        else if (action == GLFW_RELEASE) inputManager->sPressed = false;
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS) inputManager->aPressed = true;
        else if (action == GLFW_RELEASE) inputManager->aPressed = false;
        break;
    case GLFW_KEY_D:
        if (action == GLFW_PRESS) inputManager->dPressed = true;
        else if (action == GLFW_RELEASE) inputManager->dPressed = false;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        if (action == GLFW_PRESS) inputManager->ctrlPressed = true;
        else if (action == GLFW_RELEASE) inputManager->ctrlPressed = false;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        if (action == GLFW_PRESS) inputManager->shiftPressed = true;
        else if (action == GLFW_RELEASE) inputManager->shiftPressed = false;
        break;

    default:
        break;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto appWin = (application_window*)glfwGetWindowUserPointer(window);

    if (appWin->mouse_left) {
        auto xoffset = ((double)xpos - appWin->lastx) / appWin->width;
        auto yoffset = ((double)ypos - appWin->lasty) / appWin->height;
        appWin->lastx = xpos;
        appWin->lasty = ypos;

        appWin->camera->rotate(xoffset, yoffset);
    }
    if (appWin->mouse_right) {
        auto xoffset = ((double)xpos - appWin->lastx) / appWin->width;
        auto yoffset = ((double)ypos - appWin->lasty) / appWin->height;
        appWin->lastx = xpos;
        appWin->lasty = ypos;

        appWin->camera->pan(xoffset, yoffset);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto appWin = (application_window*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        appWin->mouse_left = true;
        glfwGetCursorPos(window, &appWin->lastx, &appWin->lasty);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        appWin->mouse_left = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        appWin->mouse_right = true;
        glfwGetCursorPos(window, &appWin->lastx, &appWin->lasty);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        appWin->mouse_right = false;
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto appWin = (application_window*)glfwGetWindowUserPointer(window);
    appWin->camera->dolly(yoffset / 10);
}

