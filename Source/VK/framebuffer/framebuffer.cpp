#include "framebuffer.h"

#include "../device/device.h"

namespace VK
{
    Framebuffer::Framebuffer(VkImageView imageView, VkRenderPass renderPass, const unsigned int width, const unsigned int height, VkImageView depthImageView) : size{static_cast<float>(width), static_cast<float>(height)}
    {
        std::vector<VkImageView> attachments = {imageView};

        if (depthImageView != VK_NULL_HANDLE)
        {
            attachments.push_back(depthImageView);
        }

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = renderPass;
        framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebuffer_info.pAttachments = attachments.data();
        framebuffer_info.width = static_cast<uint32_t>(size.x);
        framebuffer_info.height = static_cast<uint32_t>(size.y);
        framebuffer_info.layers = 1;

        VK_TRY(vkCreateFramebuffer(Global::device->GetVkDevice(), &framebuffer_info, nullptr, &vkFramebuffer));
    }
    
    Framebuffer::Framebuffer(VkImageView image_view, VkRenderPass render_pass, const Vec2& size) : size { size }
    {
        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = &image_view;
        framebuffer_info.width = static_cast<uint32_t>(size.x);
        framebuffer_info.height = static_cast<uint32_t>(size.y);
        framebuffer_info.layers = 1;

        VK_TRY(vkCreateFramebuffer(Global::device->GetVkDevice(), &framebuffer_info, nullptr, &vkFramebuffer));
    }

    Framebuffer::~Framebuffer()
    {
        vkDestroyFramebuffer(Global::device->GetVkDevice(), vkFramebuffer, nullptr);

        
    }

    VkFramebuffer Framebuffer::GetVkFramebuffer() const
    {
        return vkFramebuffer;
    }

    Vec2 Framebuffer::GetSize() const
    {
        return size;
    }
}