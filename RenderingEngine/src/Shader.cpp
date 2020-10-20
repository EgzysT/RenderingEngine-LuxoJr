#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Utils.h"

Shader::Shader(const std::string& vertFilePath, const std::string& fragFilePath)
	: vertFilePath(vertFilePath), fragFilePath(fragFilePath), rendererID(0)
{
    if (!std::filesystem::exists(vertFilePath)) {
        std::cout << "Error: Vertex Shader File Not Found: " << vertFilePath << std::endl;
#if _DEBUG
        __debugbreak();
#endif
    }
    if (!std::filesystem::exists(fragFilePath)) {
        std::cout << "Error: Fragment Shader File Not Found: " << fragFilePath << std::endl;
#if _DEBUG
        __debugbreak();
#endif
    }

    std::ifstream vertShaderFile;
    vertShaderFile.open(vertFilePath);
    std::stringstream vertStrStream;
    vertStrStream << vertShaderFile.rdbuf();
    std::string vertShaderSrc = vertStrStream.str();

    std::ifstream fragShaderFile;
    fragShaderFile.open(fragFilePath);
    std::stringstream fragStrStream;
    fragStrStream << fragShaderFile.rdbuf();
    std::string fragShaderSrc = fragStrStream.str();

    rendererID = CreateShader(vertShaderSrc, fragShaderSrc);
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
    glUseProgram(rendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

GLint Shader::getUniformLocation(const std::string& name)
{
    auto locationCache = uniformLocationCache.find(name);
    if (locationCache != uniformLocationCache.end())
        return locationCache->second;
    GLint location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " not found." << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);         //TODO: Error checking (documentation)

    //Delete intermidiates
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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

void Shader::SetUniformVec4(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::SetUniformInteger(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}
