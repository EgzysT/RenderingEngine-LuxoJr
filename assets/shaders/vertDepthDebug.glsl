#version 330 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
// layout(location = 2) in vec3 normals;
// layout(location = 3) in vec3 tangents;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0); 
    TexCoords = texCoords;
}  