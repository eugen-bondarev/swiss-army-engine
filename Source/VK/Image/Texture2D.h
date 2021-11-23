#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(ImageView);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Image);
    
    class Texture2D
    {
    public:
        Texture2D(Vec2 size, int amount_of_channels, const void* data = nullptr, VkImageUsageFlags usage_flags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr);
        ~Texture2D();

        Vec2 GetSize() const;
        Image* GetImage();
        ImageView* GetImageView();

    protected:
        Vec2 size;

    private:
        const Device& device;
        Image* image;
        ImageView* imageView;

        Texture2D(const Texture2D&) = delete;			
        Texture2D& operator=(const Texture2D&) = delete;			
    };
}