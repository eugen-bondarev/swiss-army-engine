#ifndef __VK_Entities_Framebuffer_Framebuffer_h__
#define __VK_Entities_Framebuffer_Framebuffer_h__

#pragma once

#include "../Device/Device.h"
#include "../../Common.h"

namespace VK
{
    class Framebuffer
    {
    public:
        Framebuffer(VkImageView imageView, VkRenderPass renderPass, const unsigned int width, const unsigned int height, const Device* device = nullptr);
       ~Framebuffer();

        const VkFramebuffer& GetVkFramebuffer() const;

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    private:
        const Device& device;
        VkFramebuffer vkFramebuffer;
        unsigned int width;
        unsigned int height;

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
    };
}

#endif