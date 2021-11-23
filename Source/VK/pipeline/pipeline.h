#pragma once

#include "RenderPass.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Shader);
    FORWARD_DECLARE(RenderPass);
    
    using BindingDescriptions = std::vector<VkVertexInputBindingDescription>;
    using AttributeDescriptions = std::vector<VkVertexInputAttributeDescription>;
    using SetLayouts = std::vector<VkDescriptorSetLayout>;

    class Pipeline
    {
    public:
        Pipeline(
            const std::string& vsCode,
            const std::string& fsCode,
            const Vec2& viewportSize,
            const AttachmentDescriptions& attachments,
            const BindingDescriptions& bindingDescriptions = {},
            const AttributeDescriptions& attributeDescriptions = {},
            const SetLayouts& setLayouts = {},
            const Device* device = nullptr
        );

        ~Pipeline();

        void SetAsOutput();

        VkPipelineLayout GetVkPipelineLayout() const;
        VkPipeline GetVkPipeline() const;
        RenderPass *GetRenderPass();

    private:
        const Device& device;
        Shader* shader;
        RenderPass* renderPass;

        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkPipeline;

        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;
    };
}