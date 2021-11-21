#ifndef __VK_Entities_Vertex_h__
#define __VK_Entities_Vertex_h__

#pragma once

#include "../Common.h"

namespace VK
{
    struct Vec2
    {
        float x{0}, y{0};
    };

    struct Vertex
    {
        Vec2 Position;
        Vec2 TextureCoords;

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    };
}

#endif