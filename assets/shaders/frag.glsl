#version 330 core
#define NUM_LIGHTS 2

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_EyeVec;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
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

        vec3 reflection = reflect(lightDir, normal);
        float specular = pow( max( dot(reflection, eyeVec), 0.0 ), u_matShininess);

        specularColor = uLight[i].specular * u_matSpecular * specular;
    }
    return texColor * (ambientColor + diffuseColor) + specularColor;
}

vec4 lighting(vec3 eyeVec, vec3 normal) {
// vec4 lighting() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        // result += uLight[i].ambient;

        result += calcDirectionalLight(i, eyeVec, normalize(uLight[i].position.xyz), normal, texColor);
    }

    result.a = 1.0;
    return result;
}

void main() {

    // Transformed normal position
	vec3 normal = normalize(v_Normal);
    vec3 eyeVec = normalize(v_EyeVec);

    vec4 illumColor = lighting(eyeVec, normal);
    // vec4 illumColor = lighting();
    // color = texColor * u_Color;
    // color = texColor;
    color = illumColor;
    // color = vec4(v_Normal, 1.0);
}