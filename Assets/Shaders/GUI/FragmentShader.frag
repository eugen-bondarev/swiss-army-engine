#version 450

layout(location = 0) in VSOut
{
    vec2 fsTexCoords;
};

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D texSampler;

void main()
{
    outColor = texture(texSampler, fsTexCoords);
}