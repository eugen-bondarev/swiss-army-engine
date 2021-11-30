#ifndef __VK_Framebuffer_Framebuffer_h__
#define __VK_Framebuffer_Framebuffer_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(RenderPass);
    FORWARD_DECLARE(ImageView);
    FORWARD_DECLARE(Device);
    
    class Framebuffer
    {
    public:
        Framebuffer(const RenderPass& renderPass, const Vec2ui size, const ImageView& imageView, const ImageView& depthImageView, const ImageView& multisampleImageView, const Device& device = GetDevice());
       ~Framebuffer();

        VkFramebuffer GetVkFramebuffer() const;
        Vec2ui GetSize() const;

    private:
        const Device& device;
        VkFramebuffer vkFramebuffer;
        Vec2ui size;

        Framebuffer(const Framebuffer &) = delete;
        Framebuffer &operator=(const Framebuffer &) = delete;
    };
}

#endif