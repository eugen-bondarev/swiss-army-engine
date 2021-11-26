#include "Layouts.h"

namespace VK
{
    std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> binding_descriptions;

        VkVertexInputBindingDescription bindingDesc = {};
        bindingDesc.binding = 0;
        bindingDesc.stride = sizeof(Vertex);
        bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        binding_descriptions.push_back(bindingDesc);

        return binding_descriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions() 
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        attributeDescriptions.resize(2, {});

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, Position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, TextureCoords);

        return attributeDescriptions;
    }
}