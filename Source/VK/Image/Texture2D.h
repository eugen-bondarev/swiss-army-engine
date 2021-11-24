#ifndef __VK_Image_Texture2D_h__
#define __VK_Image_Texture2D_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(ImageView);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Image);
    
    class Texture2D
    {
    public:
        Texture2D(const Vec2ui size, const uint32_t numChannels, const void* data = nullptr, VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device& device = GetDevice());
       ~Texture2D();

        Vec2ui GetSize() const;
        Image* GetImage();
        ImageView* GetImageView();

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