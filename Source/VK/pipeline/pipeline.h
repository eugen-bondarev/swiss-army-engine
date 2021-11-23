#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "shader.h"
#include "render_pass.h"

#include "vertex_layouts/layouts.h"

namespace VK
{
    using BindingDescriptions = std::vector<VkVertexInputBindingDescription>;
    using AttributeDescriptions = std::vector<VkVertexInputAttributeDescription>;
    using SetLayouts = std::vector<VkDescriptorSetLayout>;

    class Pipeline
    {
    public:
        Pipeline(
            const std::string &vs_code,
            const std::string &fs_code,
            const Vec2 &viewport_size,
            const AttachmentDescriptions& attachments,
            const BindingDescriptions &binding_descriptions = {},
            const AttributeDescriptions &attribute_descriptions = {},
            const SetLayouts &set_layouts = {},
            const Global::Device* device = nullptr
        );

        ~Pipeline();

        void SetAsOutput();

        VkPipelineLayout GetVkPipelineLayout() const;
        VkPipeline GetVkPipeline() const;
        RenderPass *GetRenderPass();

    private:
        const Global::Device& device;
        Shader* shader;
        RenderPass* renderPass;

        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkPipeline;

        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;
    };
}