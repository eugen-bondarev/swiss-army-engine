#include "RenderPass.h"

#include "../../GraphicsContext.h"

namespace VK
{
    namespace Util
    {			
        VkAttachmentDescription CreateAttachment(
            VkFormat format, 
            VkImageLayout initial_layout,
            VkImageLayout final_layout, 
            VkAttachmentLoadOp load_op, 
            VkAttachmentStoreOp store_op,
            VkAttachmentLoadOp stencil_load_op, 
            VkAttachmentStoreOp stencil_store_op,
            VkSampleCountFlagBits samples
        )
        {
            VkAttachmentDescription attachmentDesc = {};
            attachmentDesc.format = format;
            attachmentDesc.samples = samples;
            attachmentDesc.loadOp = load_op;
            attachmentDesc.storeOp = store_op;
            attachmentDesc.stencilLoadOp = stencil_load_op;
            attachmentDesc.stencilStoreOp = stencil_store_op;
            attachmentDesc.initialLayout = initial_layout;
            attachmentDesc.finalLayout = final_layout;
            return attachmentDesc;
        }
    }

    RenderPass::RenderPass(const AttachmentDescriptions& attachments, const Device* device) : device{device ? *device : *GetDevice()}
    {
        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;
        
        std::vector<VkSubpassDependency> dependencies;
        
        dependencies.resize(1);
        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].srcAccessMask = 0;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = static_cast<uint32_t>(dependencies.size());
        render_pass_info.pDependencies = dependencies.data();

        VK_TRY(vkCreateRenderPass(this->device.GetVkDevice(), &render_pass_info, nullptr, &vkRenderPass));
    }

    RenderPass::~RenderPass()
    {
        vkDestroyRenderPass(device.GetVkDevice(), vkRenderPass, nullptr);        
    }

    const VkRenderPass& RenderPass::GetVkRenderPass() const
    {
        return vkRenderPass;
    }
}