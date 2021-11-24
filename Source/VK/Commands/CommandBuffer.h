#ifndef __VK_Commands_CommandBuffer_h__
#define __VK_Commands_CommandBuffer_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(CommandPool);
    FORWARD_DECLARE(Framebuffer);
    FORWARD_DECLARE(RenderPass);
    FORWARD_DECLARE(Pipeline);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Buffer);

    class CommandBuffer
    {
    public:
        CommandBuffer(const CommandPool* commandPool, const Device* device = nullptr);
       ~CommandBuffer();

        void Begin(VkCommandBufferUsageFlags flags = 0) const;
        void End() const;

        void SubmitToQueue(const VkQueue& queue, const VkSemaphore* waitSemaphore = nullptr, const VkSemaphore* signalSemaphore = nullptr, VkFence fence = VK_NULL_HANDLE) const;

        void BeginRenderPass(const RenderPass& renderPass, Framebuffer* framebuffer, const std::array<float, 4>& color = {0.0f, 0.0f, 0.0f, 1.0f}) const;
        void EndRenderPass() const;

        void BindPipeline(const Pipeline* pipeline) const;

        void BindVertexBuffers(const std::vector<Buffer*>& buffers, const std::vector<VkDeviceSize>& offsets = {0}) const;
        void BindIndexBuffer(Buffer* indexBuffer, VkIndexType indexType = VK_INDEX_TYPE_UINT32) const;
        void BindDescriptorSets(const Pipeline* pipeline, uint32_t numDescriptorSets, const VkDescriptorSet* descriptorSets, uint32_t numOffsets = 0, uint32_t* offsets = nullptr) const;

        template <typename... Args>
        void Draw(Args&&... args) const
        {
            vkCmdDraw(vkCommandBuffer, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void DrawIndexed(Args&&... args) const
        {
            vkCmdDrawIndexed(vkCommandBuffer, std::forward<Args>(args)...);
        }

        void Free() const;

        const VkCommandBuffer &GetVkCommandBuffer() const;

    private:
        const CommandPool& commandPool;
        const Device& device;

        VkCommandBuffer vkCommandBuffer;

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
    };
}

#endif