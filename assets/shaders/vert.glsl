#version 330 core
#define NUM_LIGHTS 2

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;

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
out vec3 v_Tangent;
out vec3 v_LightDir[NUM_LIGHTS];

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat4 u_NormalMatrix; //transpose(inverse(MV_Matrix)) done in cpu

void main() {
    vec4 vertPos = u_ModelViewMatrix * position;
    
    v_EyeVec = -vec3(vertPos.xyz);
    
    gl_Position = u_ProjMatrix * vertPos;

    v_Normal = vec3(u_NormalMatrix * vec4(normals, 0.0));
    v_Tangent = vec3(u_NormalMatrix * vec4(tangents, 0.0));

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (uLight[i].position.w == 1.0) {
            v_LightDir[i] = vertPos.xyz - uLight[i].position.xyz;
        }
    }

    v_TexCoord = texCoords;
}