#include "InputManager.h"
#include "Time.h"
//#include "Graphics.h"

std::shared_ptr<InputManager> InputManager::instance;

InputManager* InputManager::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<InputManager>();
    }
    return instance.get();
}

void InputManager::InitInput(GLFWwindow* window)
{
    this->window = window;
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
}

void InputManager::ProcessInput()
{
    auto appWin = (application_window*)glfwGetWindowUserPointer(window);
    double deltaTime = Time::GetInstance()->deltaTime;
    double speed = 0.5;
    if (wPressed && !sPressed)
    {
        appWin->camera->moveLocalZ(speed * deltaTime);
    }
    else if (sPressed && !wPressed) {
        appWin->camera->moveLocalZ(-speed * deltaTime);
    }

    if (aPressed && !dPressed) {
        appWin->camera->pan(speed * deltaTime, 0);
    }
    else if (dPressed && !aPressed) {
        appWin->camera->pan(-speed * deltaTime, 0);
    }

    if (shiftPressed && !ctrlPressed)
    {
        appWin->camera->pan(0, speed * deltaTime);
    }
    else if (ctrlPressed && !shiftPressed) {
        appWin->camera->pan(0, -speed * deltaTime);
    }
}
