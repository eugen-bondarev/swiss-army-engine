#ifndef __VK_Pipeline_VertexLayouts_Layouts_h__
#define __VK_Pipeline_VertexLayouts_Layouts_h__

#pragma once

#include "../../common.h"

#include <glm.hpp>

namespace VK
{
    struct Vertex
    {
        Vec3f Position;
        Vec2f TextureCoords;
        Vec3f normal;

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    };

    struct Vertex2D
    {
        Vec2f position;
        Vec2f textureCoords;

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    };
}

#endif