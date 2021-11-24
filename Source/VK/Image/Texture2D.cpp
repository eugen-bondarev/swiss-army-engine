#include "texture2d.h"

#include "../Memory/Buffer.h"
#include "../GraphicsContext.h"
#include "ImageView.h"
#include "Image.h"

namespace VK
{
    Texture2D::Texture2D(const Vec2ui size, const uint32_t numChannels, const void* data, VkImageUsageFlags usageFlags, const Device& device) : device{device}, size{size}
    {
        uint32_t bufferSize = size.x * size.y * numChannels;

        VK::Buffer stagingBuffer(data, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device);
        image = CreatePtr<VK::Image>(&stagingBuffer, size, VK_FORMAT_R8G8B8A8_UNORM, usageFlags, device);
        imageView = CreatePtr<VK::ImageView>(*image, VK_IMAGE_ASPECT_COLOR_BIT, device);
    }

    Texture2D::~Texture2D()
    {
        // delete imageView;
        // delete image;
    }

    Image* Texture2D::GetImage()
    {
        return image.get();
    }

    ImageView* Texture2D::GetImageView()
    {
        return imageView.get();
    }

    Vec2ui Texture2D::GetSize() const
    {
        return size;
    }
}