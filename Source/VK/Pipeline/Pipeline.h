#ifndef __VK_Pipeline_Pipeline_h__
#define __VK_Pipeline_Pipeline_h__

#pragma once

#include "../GraphicsContext.h"
#include "RenderPass.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(RenderPass);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Shader);
    
    using AttributeDescriptions = std::vector<VkVertexInputAttributeDescription>;
    using BindingDescriptions = std::vector<VkVertexInputBindingDescription>;
    using SetLayouts = std::vector<VkDescriptorSetLayout>;

    class Pipeline
    {
    public:
        Pipeline(
            const std::string& vsCode, const std::string& fsCode,
            const Vec2ui& outputSize,
            const AttachmentDescriptions& attachments,
            const BindingDescriptions& bindingDescriptions = {},
            const AttributeDescriptions& attributeDescriptions = {},
            const SetLayouts& setLayouts = {},
            const Device& device = GetDevice()
        );

       ~Pipeline();

        const VkPipelineLayout& GetVkPipelineLayout() const;
        const VkPipeline& GetVkPipeline() const;
        const RenderPass& GetRenderPass() const;

    private:
        const Device& device;

        Ptr<Shader> shader;
        Ptr<RenderPass> renderPass;

        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkPipeline;

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
    };
}

#endif