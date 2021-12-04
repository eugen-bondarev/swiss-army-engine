#version 450

layout(location = 0) in VSOut
{
    vec2 fsTexCoords;
    vec3 fsNormal;
    vec3 fsFragPos;
};

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D texSampler;

const vec3 lightPos = vec3(10000, 5000, 1000);

const float ambientLightIntensity = 0.4;
const float diffuseColorIntensity = 1.0;

vec3 CalculateAmbientLight(vec3 color, float intensity)
{
    return intensity * color;
}

vec3 CalculateDiffuseColor(vec3 color, float intensity)
{
    vec3 normalizedNormal = normalize(fsNormal);
    vec3 lightDir = normalize(lightPos - fsFragPos);
    float diffuseFactor = max(dot(normalizedNormal, lightDir), 0.0);
    return diffuseFactor * color * intensity;
}

vec4 CalculateOutputColor(vec3 ambient, vec3 diffuse, vec4 color)
{
    return vec4(ambient + diffuse, 1.0) * color;
}

void main()
{
    vec3 ambientLightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLightColor = vec3(1.0, 1.0, 1.0);
    vec4 texturedColor = texture(texSampler, fsTexCoords);    

    vec3 ambientLight = CalculateAmbientLight(ambientLightColor, ambientLightIntensity);
    vec3 diffuseColor = CalculateDiffuseColor(diffuseLightColor, diffuseColorIntensity);

    outColor = CalculateOutputColor(ambientLight, diffuseColor, texturedColor);
}