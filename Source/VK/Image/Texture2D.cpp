#include "texture2d.h"

#include "../Memory/Buffer.h"
#include "../GraphicsContext.h"
#include "ImageView.h"
#include "Image.h"

namespace VK
{
    Texture2D::Texture2D(const Vec2ui size, const uint32_t numChannels, const void* data, const VkImageUsageFlags usageFlags, const Device& device) : device{device}, size{size}
    {
        const uint32_t bufferSize = size.x * size.y * numChannels;
        const VK::Buffer stagingBuffer(data, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device);

        image = CreatePtr<VK::Image>(size, VK_FORMAT_R8G8B8A8_UNORM, usageFlags, device);
        image->LoadFrom(stagingBuffer);
        imageView = CreatePtr<VK::ImageView>(*image, image->GetVkFormat(), VK_IMAGE_ASPECT_COLOR_BIT, device);
    }

    Texture2D::Texture2D(const Vec2ui size, const VkFormat format, const VkImageUsageFlags usageFlags, const VkImageAspectFlags aspectFlags, const Device& device) : device{device}, size{size}
    {
        image = CreatePtr<VK::Image>(size, format, usageFlags, device);
        imageView = CreatePtr<VK::ImageView>(*image, image->GetVkFormat(), aspectFlags, device);
    }

    const Image& Texture2D::GetImage() const
    {
        return *image;
    }

    const ImageView& Texture2D::GetImageView() const
    {
        return *imageView;
    }

    Vec2ui Texture2D::GetSize() const
    {
        return size;
    }
}