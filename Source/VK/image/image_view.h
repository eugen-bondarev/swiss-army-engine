#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "image.h"

#include "sampler.h"

namespace VK
{
    class ImageView
    {
    public:
        ImageView(Image* image, const VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, const Global::Device* device = nullptr);
        ~ImageView();

        VkImageView& GetVkImageView();

        VkDescriptorImageInfo& GetDescriptor();

        void SetupDescriptor(VkImageLayout image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VkSampler sampler = Global::constantInterpolationSampler->GetVkSampler());

    private:
        const Global::Device& device;
        VkImageView vkImageView;

        VkDescriptorImageInfo descriptor;
        void SetupDefaultDescriptor();

        ImageView(const ImageView&) = delete;
        ImageView& operator=(const ImageView&) = delete;
    };
}