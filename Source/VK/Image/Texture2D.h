#ifndef __VK_Image_Texture2D_h__
#define __VK_Image_Texture2D_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"
#include "ImageView.h"
#include "Image.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
    class Texture2D
    {
    public:
        Texture2D(const Vec2ui size, const uint32_t numChannels, const void* data = nullptr, const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device& device = GetDevice());
        Texture2D(const Vec2ui size, const VkFormat format, const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, const VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT, const Device& device = GetDevice());
       ~Texture2D() = default;

        Vec2ui GetSize() const;
        const Image& GetImage() const;
        const ImageView& GetImageView() const;

    protected:
        Vec2ui size;

    private:
        const Device& device;
        Ptr<Image> image;
        Ptr<ImageView> imageView;

        Texture2D(const Texture2D&) = delete;			
        Texture2D& operator=(const Texture2D&) = delete;			
    };
}

#endif