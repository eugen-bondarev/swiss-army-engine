#include "Pipeline.h"

#include "../GraphicsContext.h"
#include "../Device/Device.h"
#include "Shader.h"

namespace VK
{
    Pipeline::Pipeline(
        const Str& vsCode,
        const Str& fsCode,
        const Vec2ui& outputSize,
        const AttachmentDescriptions& attachments,
        const BindingDescriptions& bindingDescriptions,
        const AttributeDescriptions& attributeDescriptions,
        const SetLayouts& setLayouts,
        const Device& device
    ) : device{device}
    {
        shader = CreatePtr<Shader>(vsCode, fsCode);

        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
        inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = outputSize.x;
        viewport.height = outputSize.y;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {static_cast<uint32_t>(outputSize.x), static_cast<uint32_t>(outputSize.y)};

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.pViewports = &viewport;
        viewportStateCreateInfo.scissorCount = 1;
        viewportStateCreateInfo.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo{};
        rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerCreateInfo.depthClampEnable = VK_FALSE;
        rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerCreateInfo.lineWidth = 1.0f;
        rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        // rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizerCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizerCreateInfo.depthBiasClamp = 0.0f;		   // Optional
        rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;	   // Optional

        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f;			// Optional
        multisampleStateCreateInfo.pSampleMask = nullptr;			// Optional
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;		// Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;  				// Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; 		// Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;			   						// Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;  				// Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; 		// Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;			   						// Optional

        VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo{};
        colorBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendingCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendingCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlendingCreateInfo.attachmentCount = 1;
        colorBlendingCreateInfo.pAttachments = &colorBlendAttachment;
        colorBlendingCreateInfo.blendConstants[0] = 0.0f; // Optional
        colorBlendingCreateInfo.blendConstants[1] = 0.0f; // Optional
        colorBlendingCreateInfo.blendConstants[2] = 0.0f; // Optional
        colorBlendingCreateInfo.blendConstants[3] = 0.0f; // Optional

        const std::array<VkDynamicState, 2> dynamicStates = 
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;								 // Optional
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;								 // Optional
        VK_TRY(vkCreatePipelineLayout(this->device.GetVkDevice(), &pipelineLayoutCreateInfo, nullptr, &vkPipelineLayout));

        renderPass = CreatePtr<RenderPass>(attachments, device);

        VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo{};
        depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilCreateInfo.depthTestEnable = VK_TRUE;
        depthStencilCreateInfo.depthWriteEnable = VK_TRUE;
        depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilCreateInfo.minDepthBounds = 0.0f; // Optional
        depthStencilCreateInfo.maxDepthBounds = 1.0f; // Optional
        depthStencilCreateInfo.stencilTestEnable = VK_FALSE;
        depthStencilCreateInfo.front = {}; // Optional
        depthStencilCreateInfo.back = {}; // Optional

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.stageCount = static_cast<uint32_t>(shader->GetStages().size());
        createInfo.pStages = shader->GetStages().data();
        createInfo.pVertexInputState = &vertexInputStateCreateInfo;
        createInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
        createInfo.pViewportState = &viewportStateCreateInfo;
        createInfo.pRasterizationState = &rasterizerCreateInfo;
        createInfo.pMultisampleState = &multisampleStateCreateInfo;
        createInfo.pDepthStencilState = &depthStencilCreateInfo; // Optional
        createInfo.pColorBlendState = &colorBlendingCreateInfo;
        createInfo.pDynamicState = nullptr; // Optional
        createInfo.layout = vkPipelineLayout;
        createInfo.renderPass = renderPass->GetVkRenderPass();
        createInfo.subpass = 0;
        createInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        createInfo.basePipelineIndex = -1;			   // Optional

        VK_TRY(vkCreateGraphicsPipelines(this->device.GetVkDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &vkPipeline));        
    }

    Pipeline::~Pipeline()
    {
        vkDestroyPipeline(device.GetVkDevice(), vkPipeline, nullptr);
        vkDestroyPipelineLayout(device.GetVkDevice(), vkPipelineLayout, nullptr);
    }

    const VkPipelineLayout& Pipeline::GetVkPipelineLayout() const
    {
        return vkPipelineLayout;
    }

    const VkPipeline& Pipeline::GetVkPipeline() const
    {
        return vkPipeline;
    }

    const RenderPass& Pipeline::GetRenderPass() const
    {
        return *renderPass;
    }
}