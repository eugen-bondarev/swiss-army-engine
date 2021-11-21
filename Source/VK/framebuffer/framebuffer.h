#pragma once

#include "../Common.h"

namespace Engine
{
	namespace Vk
	{
		class Framebuffer
		{
		public:
			Framebuffer(VkImageView image_view, VkRenderPass render_pass, const Vec2 &size);
			~Framebuffer();

			VkFramebuffer GetVkFramebuffer() const;
			Vec2 GetSize() const;

		private:
			VkFramebuffer vkFramebuffer;
			Vec2 size;

			Framebuffer(const Framebuffer &) = delete;
			Framebuffer &operator=(const Framebuffer &) = delete;
		};
	}
}