#include "ImageView.h"

#include "../Device/Device.h"
#include "Sampler.h"
#include "Image.h"

namespace VK
{
    ImageView::ImageView(const VkImageView& vkImageView, const Device& device) : device{device}, vkImageView{vkImageView}
    {
        SetupDefaultDescriptor();
    }

    ImageView::ImageView(const Image& image, const VkFormat format, const VkImageAspectFlags aspectFlags, const Device& device) : device{device}
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image.GetVkImage();
        createInfo.format = format;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        VK_TRY(vkCreateImageView(this->device.GetVkDevice(), &createInfo, nullptr, &vkImageView));
        SetupDefaultDescriptor();
    }

    ImageView::~ImageView()
    {
        vkDestroyImageView(device.GetVkDevice(), vkImageView, nullptr);   
    }

    const VkImageView& ImageView::GetVkImageView() const
    {
        return vkImageView;
    }

    void ImageView::SetupDefaultDescriptor()
    {
        vkDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        vkDescriptor.imageView = vkImageView;
        vkDescriptor.sampler = GetDefaultConstInterpolationSampler().GetVkSampler();
    }

    void ImageView::SetupDescriptor(const VkImageLayout imageLayout, const Sampler& sampler)
    {
        vkDescriptor.imageLayout = imageLayout;
        vkDescriptor.sampler = sampler.GetVkSampler();
    }

    const VkDescriptorImageInfo& ImageView::GetVkDescriptor() const
    {
        return vkDescriptor;
    }
}