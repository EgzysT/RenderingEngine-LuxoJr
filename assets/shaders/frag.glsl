#version 330 core
#define NUM_LIGHTS 1

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_EyeVec;
in vec3 v_Tangent;
in vec4 v_FragPosLightSpace;
in vec3 v_LightDir[NUM_LIGHTS];

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform sampler2D u_NormalMap;
// uniform sampler2D u_ShadowMap;
uniform sampler2DShadow u_ShadowMap;
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

// Returns 1.0 if is in shadow, 0.0 if not in shadow
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    vec3 shadowTexCoord = vec3(projCoords.xy, (projCoords.z)/fragPosLightSpace.w);
    float closestDepth = texture(u_ShadowMap, shadowTexCoord);
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.002);
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    float shadow = 0.0;
    // vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0));
    vec3 texelSize = vec3(vec2(1.0 / textureSize(u_ShadowMap, 0)), 1);

    int samples = 1;
    for(int x = -samples; x <= samples; ++x)
    {
        for(int y = -samples; y <= samples; ++y)
        {
            // float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            float pcfDepth = texture(u_ShadowMap, shadowTexCoord + vec3(x, y, 0) * texelSize);

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    float sampleSqr = float((samples * 2 + 1) * (samples * 2 + 1));
    shadow /= sampleSqr;

    return shadow;
}

vec4 calcDirectionalLight(int i, vec3 eyeVec, vec3 lightDir, vec3 normal, vec4 texColor, float shadowCalc) {
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

    float shadow = ShadowCalculation(v_FragPosLightSpace, lightDir, normal);

    vec4 color = texColor * (ambientColor + ((1.0 - max(shadow, shadowCalc)) * diffuseColor)) 
                + ((1.0 - max(shadow, shadowCalc)) * specularColor);
    return color;
}

vec4 calcPointLight(int i, vec3 eyeVec, vec3 normal, vec4 texColor) {
    float dist = length(v_LightDir[i]);
    vec3 dir = normalize(v_LightDir[i]);

    vec4 color = calcDirectionalLight(i, eyeVec, dir, normal, texColor, 1.0);
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
            result += calcDirectionalLight(i, eyeVec, normalize(uLight[i].position.xyz), normal, texColor, 0.0);
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