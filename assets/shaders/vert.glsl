#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec2 normals;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjMatrix;
uniform mat4 u_modelMatrix;

void main() {
    gl_Position = u_ViewProjMatrix * u_modelMatrix * position;
    v_TexCoord = texCoords;
}