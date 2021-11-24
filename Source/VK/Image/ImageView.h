#ifndef __VK_Image_ImageView_h__
#define __VK_Image_ImageView_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Sampler);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Image);
    
    class ImageView
    {
    public:
        ImageView(const Image& image, const VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, const Device* device = nullptr);
        ~ImageView();

        VkImageView& GetVkImageView();

        VkDescriptorImageInfo& GetDescriptor();

        void SetupDescriptor(VkImageLayout image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, const Sampler& sampler = GetDefaultConstInterpolationSampler());

    private:
        const Device& device;
        VkImageView vkImageView;

        VkDescriptorImageInfo descriptor;
        void SetupDefaultDescriptor();

        ImageView(const ImageView&) = delete;
        ImageView& operator=(const ImageView&) = delete;
    };
}

#endif