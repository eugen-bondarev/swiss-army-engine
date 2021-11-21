#pragma once

#include "../../common.h"

namespace Engine
{
	namespace Vk
	{
		struct Vertex
		{
			Vec2 Position;
			Vec2 TextureCoords;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		// struct PerInstanceVertex
		// {
		// 	glm::vec4 Position;

		// 	static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
		// 	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		// };

		struct PerInstanceVertex2D
		{
			Vec2 Position;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};
	}
}