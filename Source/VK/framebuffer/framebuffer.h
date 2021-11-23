#pragma once

#include "../Common.h"


namespace VK
{
    FORWARD_DECLARE(Device);
    
    class Framebuffer
    {
    public:
        Framebuffer(VkImageView image_view, VkRenderPass renderPass, const unsigned int width, const unsigned int height, VkImageView depthImageView = VK_NULL_HANDLE, const Device* device = nullptr);
        Framebuffer(VkImageView image_view, VkRenderPass renderPass, const Vec2 &size, const Device* device = nullptr);
        ~Framebuffer();

        VkFramebuffer GetVkFramebuffer() const;
        Vec2 GetSize() const;

    private:
        const Device& device;
        VkFramebuffer vkFramebuffer;
        Vec2 size;

        Framebuffer(const Framebuffer &) = delete;
        Framebuffer &operator=(const Framebuffer &) = delete;
    };
}