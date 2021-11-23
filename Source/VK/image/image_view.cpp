#include "image_view.h"

#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{
    ImageView::ImageView(Image* image, const VkImageAspectFlags aspectFlags, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = image->GetVkImage();
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        // create_info.format = VK_FORMAT_R8G8B8A8_SRGB;
        create_info.format = image->GetVkFormat();
        create_info.subresourceRange.aspectMask = aspectFlags;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VK_TRY(vkCreateImageView(this->device.GetVkDevice(), &create_info, nullptr, &vkImageView));

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
        descriptor.sampler = constantInterpolationSampler->GetVkSampler();
    }

    void ImageView::SetupDescriptor(VkImageLayout image_layout, VkSampler sampler)
    {
        descriptor.imageLayout = image_layout;
        descriptor.sampler = sampler;
    }

    VkDescriptorImageInfo& ImageView::GetDescriptor()
    {
        return descriptor;
    }
}