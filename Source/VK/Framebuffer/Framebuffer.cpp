#include "framebuffer.h"

#include "../Pipeline/RenderPass.h"
#include "../GraphicsContext.h"
#include "../Image/ImageView.h"
#include "../Device/Device.h"

namespace VK
{
    Framebuffer::Framebuffer(const RenderPass& renderPass, const Vec2ui size, const ImageView& imageView, const ImageView* depthImageView, const ImageView* multisampleImageView, const Device& device) : device{device}, size{size}
    {
        std::vector<VkImageView> attachments;
        const bool useDepth {!!depthImageView};
        const bool useMultisample {!!multisampleImageView};

        if (useDepth && !useMultisample)
        {
            attachments = {imageView.GetVkImageView(), depthImageView->GetVkImageView()};
        }

        if (useDepth && useMultisample)
        {
            attachments = {multisampleImageView->GetVkImageView(), depthImageView->GetVkImageView(), imageView.GetVkImageView()};
        }

        if (!useDepth && !useMultisample)
        {
            attachments = {imageView.GetVkImageView()};
        }

        if (!useDepth && useMultisample)
        {
            attachments = {multisampleImageView->GetVkImageView(), imageView.GetVkImageView()};
        }

        VkFramebufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = renderPass.GetVkRenderPass();
        createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        createInfo.pAttachments = attachments.data();
        createInfo.width = static_cast<uint32_t>(size.x);
        createInfo.height = static_cast<uint32_t>(size.y);
        createInfo.layers = 1;
        VK_TRY(vkCreateFramebuffer(this->device.GetVkDevice(), &createInfo, nullptr, &vkFramebuffer));
    }

    Framebuffer::~Framebuffer()
    {
        vkDestroyFramebuffer(device.GetVkDevice(), vkFramebuffer, nullptr);
    }

    VkFramebuffer Framebuffer::GetVkFramebuffer() const
    {
        return vkFramebuffer;
    }

    Vec2ui Framebuffer::GetSize() const
    {
        return size;
    }
}