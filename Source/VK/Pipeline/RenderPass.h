#ifndef __VK_Pipeline_RenderPass_h__
#define __VK_Pipeline_RenderPass_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

inline constexpr VkSampleCountFlagBits SamplesToVKFlags(const size_t samples)
{
    if (samples == 0 || samples == 1) return VK_SAMPLE_COUNT_1_BIT;
    if (samples == 2) return VK_SAMPLE_COUNT_2_BIT;
    if (samples == 4) return VK_SAMPLE_COUNT_4_BIT;
    if (samples == 8) return VK_SAMPLE_COUNT_8_BIT;
    if (samples == 16) return VK_SAMPLE_COUNT_16_BIT;
    if (samples == 32) return VK_SAMPLE_COUNT_32_BIT;
    if (samples == 64) return VK_SAMPLE_COUNT_64_BIT;
    return VK_SAMPLE_COUNT_1_BIT;
}

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

        VkAttachmentDescription CreateDefaultDepthAttachment(const VkFormat& depthImageFormat, const VkSampleCountFlagBits samples);
    }

    using AttachmentDescriptions = std::vector<VkAttachmentDescription>;

    class RenderPass
    {
    public:
        RenderPass(const AttachmentDescriptions& attachments, const size_t samples, const bool useDepth, const Device& device = GetDevice());
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