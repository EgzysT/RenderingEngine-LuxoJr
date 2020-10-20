#define STB_IMAGE_IMPLEMENTATION //THIS DEFINITION IS NECESSARY ONCE, READ THE FILE
#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<            
            " shader: " << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);         //TODO: Error checking (documentation)

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1600, 900, "Real-time Rendering 2020", nullptr, nullptr);
    if (!window)
    {
        ERROR_EXIT("Failed to create window or context");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //TODO: This activates vsync
    glfwSwapInterval(1);

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

    glfwSetKeyCallback(window, key_callback);

    // TEMP BELOW
    float vertices[] = {
        //positions    //texture coords
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,   0.0f, 1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    IndexBuffer indexBuffer(indices, 6);

    //TODO: The paths are relative to the project root when running inside VS, might not work in standalone
    Shader shader("..\\assets\\shaders\\vert.glsl", "..\\assets\\shaders\\frag.glsl");
    shader.Bind();

    Texture texture("..\\assets\\textures\\test.png");
    texture.Bind();
    shader.SetUniformInteger("u_Texture", 0);

    float r = 0.0f;
    float increment = 0.035f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.SetUniformVec4("u_Color", r, 0.3f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (r < 0.0f) {
            increment = 0.035f;
        }
        else if (r > 1.0f) {
            increment = -0.035f;
        }
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}