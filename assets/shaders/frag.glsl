#version 330 core
#define NUM_LIGHTS 2

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_EyeVec;
in vec3 v_Tangent;
in vec3 v_LightDir[NUM_LIGHTS];

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform sampler2D u_NormalMap;
uniform float u_matSpecular;
uniform float u_matShininess;

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

vec4 calcDirectionalLight(int i, vec3 eyeVec, vec3 lightDir, vec3 normal, vec4 texColor) {
    vec4 ambientColor = uLight[i].ambient;
    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    float diffuseFactor = dot(normal, -lightDir); //lambert term

    if (diffuseFactor > 0) {
        diffuseColor = uLight[i].diffuse * diffuseFactor;
    }

    vec3 reflection = reflect(lightDir, normal);
    float specular = pow( max( dot(reflection, eyeVec), 0.0 ), u_matShininess);

    specularColor = uLight[i].specular * u_matSpecular * specular;

    return texColor * (ambientColor + diffuseColor) + specularColor;
}

vec4 calcPointLight(int i, vec3 eyeVec, vec3 normal, vec4 texColor) {
    float dist = length(v_LightDir[i]);
    vec3 dir = normalize(v_LightDir[i]);

    vec4 color = calcDirectionalLight(i, eyeVec, dir, normal, texColor);
    float att = uLight[i].constant_attenuation + 
                uLight[i].linear_attenuation * dist + 
                uLight[i].quadratic_attenuation * dist * dist;
    return color / att;
}

vec4 lighting(vec3 eyeVec, vec3 normal) {
// vec4 lighting() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (uLight[i].position.w == 0.0) {
            // Directional Light
            result += calcDirectionalLight(i, eyeVec, normalize(uLight[i].position.xyz), normal, texColor);
        } else {
            // Point Light
            result += calcPointLight(i, eyeVec, normal, texColor);
        }
    }

    result.a = 1.0;
    return result;
}

vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(v_Normal);
    vec3 tangent = normalize(v_Tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpMapNormal = texture(u_NormalMap, v_TexCoord).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    newNormal = normalize(TBN * bumpMapNormal);
    return newNormal;
}

void main() {

    // Transformed normal position
	// vec3 normal = normalize(v_Normal);
    vec3 normal = CalcBumpedNormal();
    vec3 eyeVec = normalize(v_EyeVec);

    // color = texColor * u_Color;
    // color = texColor;
    color = lighting(eyeVec, normal);
    // color = vec4(v_Normal, 1.0);
}