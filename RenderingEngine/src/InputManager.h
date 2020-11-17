#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Application.h"
#include "Graphics.h"

class Application;
struct application_window;

class InputManager
{
private:

public:
	InputManager();
	void InitInput(GLFWwindow* window);
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

