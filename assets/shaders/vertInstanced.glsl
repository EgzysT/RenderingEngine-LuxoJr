#version 330 core
#define NUM_LIGHTS 1

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;
layout(location = 4) in mat4 instanceMatrix;

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
// out vec4 v_FragPosLightSpace;
out vec3 v_LightDir[NUM_LIGHTS];

// uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat4 u_NormalMatrix; //transpose(inverse(MV_Matrix)) done in cpu
// uniform mat4 u_LightSpaceMatrix;    // Light Projection * Light View

void main() {
    vec4 fragPos = instanceMatrix * vec4(vec3(position), 1.0);
    // vec4 fragPos = u_ModelMatrix * vec4(vec3(position), 1.0);
    vec4 vertPos = u_ViewMatrix * fragPos;
    
    v_EyeVec = -vec3(vertPos.xyz);
    
    vec4 pos = u_ProjMatrix * vertPos;

    mat4 mvMatrix = u_ViewMatrix * instanceMatrix;
    mat4 normalMat = transpose(inverse(mvMatrix));
    v_Normal = vec3(normalMat * vec4(normals, 0.0));
    v_Tangent = vec3(normalMat * vec4(tangents, 0.0));
    // v_Normal = vec3(u_NormalMatrix * vec4(normals, 0.0));
    // v_Tangent = vec3(u_NormalMatrix * vec4(tangents, 0.0));
    // v_FragPosLightSpace = u_LightSpaceMatrix * fragPos;

    gl_Position = pos;

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (uLight[i].position.w == 1.0) {
            v_LightDir[i] = vertPos.xyz - uLight[i].position.xyz;
        }
    }

    v_TexCoord = texCoords;
}