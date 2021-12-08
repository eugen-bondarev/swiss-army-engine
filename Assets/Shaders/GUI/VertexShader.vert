#version 450

layout(binding = 0) uniform SceneUBO {
    mat4 proj;
    mat4 view;
} sceneUBO;

layout(binding = 1) uniform EntityUBO_dyn {
    mat4 model;
} entityUBO;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out VSOut
{
    vec2 fsTexCoords;
};

void main() 
{
    gl_Position = sceneUBO.proj * sceneUBO.view * entityUBO.model * vec4(inPosition, 0.0, 1.0);
	fsTexCoords = inTexCoords;
}