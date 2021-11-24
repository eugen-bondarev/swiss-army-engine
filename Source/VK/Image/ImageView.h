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
        ImageView(const VkImageView& vkImageView, const Device& device);
        ImageView(const Image& image, const VkFormat format, const VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, const Device& device = GetDevice());
       ~ImageView();

        const VkImageView& GetVkImageView() const;
        const VkDescriptorImageInfo& GetVkDescriptor() const;

        void SetupDescriptor(const VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, const Sampler& sampler = GetDefaultConstInterpolationSampler());

    private:
        const Device& device;

        VkImageView vkImageView;
        VkDescriptorImageInfo vkDescriptor;

        void SetupDefaultDescriptor();

        ImageView(const ImageView&) = delete;
        ImageView& operator=(const ImageView&) = delete;
    };
}

#endif