#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "../framebuffer/framebuffer.h"

namespace VK
{
    namespace Util
    {
        VkAttachmentDescription CreateAttachment(
            VkFormat format, 
            VkImageLayout initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
            VkImageLayout final_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
            VkAttachmentLoadOp load_op = VK_ATTACHMENT_LOAD_OP_CLEAR, 
            VkAttachmentStoreOp store_op = VK_ATTACHMENT_STORE_OP_STORE,
            VkAttachmentLoadOp stencil_load_op = VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
            VkAttachmentStoreOp stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT
        );
    }

    using AttachmentDescriptions = std::vector<VkAttachmentDescription>;

    class RenderPass
    {
    public:
        RenderPass(const AttachmentDescriptions& attachments, const Device* device = nullptr);
        ~RenderPass();

        VkRenderPass& GetVkRenderPass();

    private:
        const Device& device;
        VkRenderPass vkRenderPass;

        RenderPass(const RenderPass &) = delete;
        RenderPass &operator=(const RenderPass &) = delete;
    };
}