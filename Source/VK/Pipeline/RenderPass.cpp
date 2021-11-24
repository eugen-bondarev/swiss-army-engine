#include "RenderPass.h"

#include "../Framebuffer/Framebuffer.h"
#include "../GraphicsContext.h"
#include "../Device/Device.h"

namespace VK
{
    namespace Util
    {			
        VkAttachmentDescription CreateAttachment(
            const VkFormat format, 
            const VkImageLayout initialLayout,
            const VkImageLayout finalLayout, 
            const VkAttachmentLoadOp loadOp, 
            const VkAttachmentStoreOp storeOp,
            const VkAttachmentLoadOp stencilLoadOp, 
            const VkAttachmentStoreOp stencilStoreOp,
            const VkSampleCountFlagBits samples
        )
        {
            VkAttachmentDescription attachment{};
            attachment.format = format;
            attachment.samples = samples;
            attachment.loadOp = loadOp;
            attachment.storeOp = storeOp;
            attachment.stencilLoadOp = stencilLoadOp;
            attachment.stencilStoreOp = stencilStoreOp;
            attachment.initialLayout = initialLayout;
            attachment.finalLayout = finalLayout;
            return attachment;
        }
    }

    RenderPass::RenderPass(const AttachmentDescriptions& attachments, const Device& device) : device{device}
    {
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        
        std::vector<VkSubpassDependency> dependencies;
        dependencies.resize(1);
        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].srcAccessMask = 0;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassCreateInfo.pAttachments = attachments.data();
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;
        renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassCreateInfo.pDependencies = dependencies.data();
        VK_TRY(vkCreateRenderPass(this->device.GetVkDevice(), &renderPassCreateInfo, nullptr, &vkRenderPass));
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