#include "texture2d.h"

#include "../Memory/Buffer.h"
#include "../GraphicsContext.h"
#include "ImageView.h"
#include "Image.h"

namespace VK
{
    Texture2D::Texture2D(Vec2 size, int amount_of_channels, const void* data, VkImageUsageFlags usage_flags, const Device* device) : device{device ? *device : GetDevice()}, size{size}
    {
        uint32_t buffer_size = size.x * size.y * amount_of_channels;

        VK::Buffer staging_buffer(data, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device);
        image = new VK::Image(&staging_buffer, size, amount_of_channels, usage_flags, device);
        imageView = new VK::ImageView(image, VK_IMAGE_ASPECT_COLOR_BIT, device);
    }

    Texture2D::~Texture2D()
    {
        delete imageView;
        delete image;
    }

    Image* Texture2D::GetImage()
    {
        return image;
    }

    ImageView* Texture2D::GetImageView()
    {
        return imageView;
    }

    Vec2 Texture2D::GetSize() const
    {
        return size;
    }
}