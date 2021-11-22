#pragma once

#include "../Common.h"

namespace VK
{
    class Framebuffer
    {
    public:
        Framebuffer(VkImageView image_view, VkRenderPass renderPass, const unsigned int width, const unsigned int height, VkImageView depthImageView = VK_NULL_HANDLE);
        Framebuffer(VkImageView image_view, VkRenderPass renderPass, const Vec2 &size);
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