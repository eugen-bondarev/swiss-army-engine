#include "Framebuffer.h"

#include "../../GraphicsContext.h"

namespace VK
{
    Framebuffer::Framebuffer(VkImageView imageView, VkRenderPass renderPass, const unsigned int width, const unsigned int height, const Device* device) : device{device ? *device : *GetDevice()}, width{width}, height{height}
    {
        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = &imageView;
        framebufferCreateInfo.width = static_cast<uint32_t>(width);
        framebufferCreateInfo.height = static_cast<uint32_t>(height);
        framebufferCreateInfo.layers = 1;

        VK_TRY(vkCreateFramebuffer(this->device.GetVkDevice(), &framebufferCreateInfo, nullptr, &vkFramebuffer));
    }

    Framebuffer::~Framebuffer()
    {
        vkDestroyFramebuffer(device.GetVkDevice(), vkFramebuffer, nullptr);        
    }

    const VkFramebuffer& Framebuffer::GetVkFramebuffer() const
    {
        return vkFramebuffer;
    }

    unsigned int Framebuffer::GetWidth() const
    {
        return width;
    }

    unsigned int Framebuffer::GetHeight() const
    {
        return height;
    }
}