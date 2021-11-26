#version 450

layout(binding = 0) uniform SceneUBO {
    mat4 proj;
} sceneUBO;

layout(binding = 1) uniform ObjectUBO_dyn {
    mat4 model;
    float another;
} dynamicUBO;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;

void main() 
{
    gl_Position = sceneUBO.proj * dynamicUBO.model * vec4(inPosition.x, inPosition.y + dynamicUBO.another, inPosition.z, 1.0);
	outTexCoords = inTexCoords;
}