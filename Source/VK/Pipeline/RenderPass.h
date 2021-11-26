#ifndef __VK_Pipeline_RenderPass_h__
#define __VK_Pipeline_RenderPass_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Framebuffer);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Image);

    namespace Util
    {
        VkAttachmentDescription CreateAttachment(
            const VkFormat format, 
            const VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            const VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
            const VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, 
            const VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            const VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
            const VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            const VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT
        );

        VkAttachmentDescription CreateDefaultDepthAttachment(const Image& depthImage);
    }

    using AttachmentDescriptions = std::vector<VkAttachmentDescription>;

    class RenderPass
    {
    public:
        RenderPass(const AttachmentDescriptions& attachments, const Device& device = GetDevice());
       ~RenderPass();

        const VkRenderPass& GetVkRenderPass() const;

    private:
        const Device& device;
        VkRenderPass vkRenderPass;

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
    };
}

#endif