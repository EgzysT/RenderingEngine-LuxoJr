#version 330 core
#define NUM_LIGHTS 2

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;

struct lightProperties {
    vec4 position;                  // Default: (0, 0, 1, 0)
    vec4 ambient;                   // Default: (0, 0, 0, 1)
    vec4 diffuse;                   // Default: (0, 0, 0, 1)
    vec4 specular;                  // Default: (0, 0, 0, 1)
    // vec4 half_vector;
    float constant_attenuation;     // Default: 1 (value must be >= 0)
    float linear_attenuation;       // Default: 0 (value must be >= 0)
    float quadratic_attenuation;    // Default: 0 (value must be >= 0)
    // bool enabled;                   // Deafult: false
};
uniform lightProperties uLight[NUM_LIGHTS];

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_EyeVec;

uniform mat4 u_ViewProjMatrix;
uniform mat4 u_modelMatrix;
uniform mat4 u_NormalMatrix; //transpose(inverse(view * model)) DO IN CPU

void main() {
    vec4 vertPos = u_ViewProjMatrix * u_modelMatrix * position;
    v_TexCoord = texCoords;

    v_Normal = vec3(u_NormalMatrix * vec4(normals, 1.0));
    // v_Normal = normals;

    v_EyeVec = -vec3(vertPos.xyz);
    
    gl_Position = vertPos;

    // for (int i = 0; i < NUM_LIGHTS; i++) {
        // if (uLight[i].position.w == 1.0) {
            // vLightDir[i] = (uMVMatrix * vec4(aVertexPosition, 1.0)).xyz - uLight[i].position.xyz;
        // }
    // }
}