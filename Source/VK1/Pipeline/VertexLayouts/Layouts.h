#pragma once

#include "../../common.h"

#include <glm.hpp>

namespace VK
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec2 TextureCoords;

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    };
}