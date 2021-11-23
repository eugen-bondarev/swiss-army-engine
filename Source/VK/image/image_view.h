#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "image.h"

#include "../GraphicsContext.h"

namespace VK
{
    FORWARD_DECLARE(Image);
    FORWARD_DECLARE(Sampler);
    
    class ImageView
    {
    public:
        ImageView(Image* image, const VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, const Device* device = nullptr);
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