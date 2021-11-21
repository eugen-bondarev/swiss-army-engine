#ifndef __VK_Entities_Commands_CommandBuffer_h__
#define __VK_Entities_Commands_CommandBuffer_h__

#pragma once

#include "../Device/Device.h"
#include "../Framebuffer/Framebuffer.h"
#include "../Pipeline/RenderPass.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"
#include "../../Common.h"
#include "CommandPool.h"

namespace VK
{
    class CommandBuffer
    {
    public:
        CommandBuffer(const CommandPool& commandPool, const Device* device = nullptr);
       ~CommandBuffer();

        void Begin(VkCommandBufferUsageFlags flags = 0);
        void End();

        void BeginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer, const std::array<float, 4>& clearColor = {0.0f, 0.0f, 0.0f, 1.0f}) const;
        void EndRenderPass() const;
        void BindPipeline(const Pipeline* pipeline) const;
        void BindVertexBuffers(const std::vector<VertexBuffer*>& buffers, const std::vector<VkDeviceSize>& offsets = {0}) const;
        void BindIndexBuffer(IndexBuffer* indexBuffer, VkIndexType indexType = VK_INDEX_TYPE_UINT32) const;
        void BindDescriptorSets(const Pipeline* pipeline, uint32_t numDescriptorsSets, VkDescriptorSet* descriptorSets, uint32_t numOffsets = 0, uint32_t* offsets = nullptr) const;

        void SubmitToQueue(const VkQueue &queue, VkSemaphore *wait_semaphore = nullptr, const VkSemaphore *signal_semaphore = nullptr, VkFence fence = nullptr) const;

        const VkCommandBuffer& GetVkCommandBuffer() const;

    private:
        const Device& device;
        const CommandPool& commandPool;

        VkCommandBuffer vkCommandBuffer;
    };
}

#endif