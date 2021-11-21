#ifndef __VK_Entities_Pipeline_Pipeline_h__
#define __VK_Entities_Pipeline_Pipeline_h__

#pragma once

#include "../../Common.h"
#include "RenderPass.h"
#include "Shader.h"

namespace VK
{    
    using BindingDescriptions = std::vector<VkVertexInputBindingDescription>;
    using AttributeDescriptions = std::vector<VkVertexInputAttributeDescription>;
    using SetLayouts = std::vector<VkDescriptorSetLayout>;

    class Pipeline
    {
    public:
        Pipeline(
            const std::string&              vsCode,
            const std::string&              fsCode,
            const unsigned int              outputWidth,
            const unsigned int              outputHeight,
            const AttachmentDescriptions&   attachments,
            const BindingDescriptions&      bindingDescriptions = {},
            const AttributeDescriptions&    attributeDescriptions = {},
            const SetLayouts&               setLayouts = {},
            const Device*                   device = nullptr
        );
       ~Pipeline();

        const VkPipeline& GetVkPipeline() const;
        const VkPipelineLayout& GetVkPipelineLayout() const;

        RenderPass* GetRenderPass();

    private:
        const Device& device;
        VkPipeline vkPipeline;
        VkPipelineLayout vkPipelineLayout;
        Ptr<Shader> shader;
        Ptr<RenderPass> renderPass;

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
    };
}

#endif