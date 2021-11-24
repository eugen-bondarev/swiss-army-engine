#include "ImageView.h"

#include "../Device/Device.h"
#include "Sampler.h"
#include "Image.h"

namespace VK
{
    ImageView::ImageView(const Image& image, const VkImageAspectFlags aspectFlags, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image.GetVkImage();
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = image.GetVkFormat();
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        VK_TRY(vkCreateImageView(this->device.GetVkDevice(), &createInfo, nullptr, &vkImageView));
        SetupDefaultDescriptor();
    }

    ImageView::~ImageView()
    {
        vkDestroyImageView(device.GetVkDevice(), vkImageView, nullptr);   
    }

    VkImageView& ImageView::GetVkImageView()
    {
        return vkImageView;
    }

    void ImageView::SetupDefaultDescriptor()
    {
        descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        descriptor.imageView = vkImageView;
        descriptor.sampler = GetDefaultConstInterpolationSampler().GetVkSampler();
    }

    void ImageView::SetupDescriptor(VkImageLayout image_layout, const Sampler& sampler)
    {
        descriptor.imageLayout = image_layout;
        descriptor.sampler = sampler.GetVkSampler();
    }

    VkDescriptorImageInfo& ImageView::GetDescriptor()
    {
        return descriptor;
    }
}