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


        void SubmitToQueue(const VkQueue &queue, VkSemaphore *wait_semaphore = nullptr, const VkSemaphore *signal_semaphore = nullptr, VkFence fence = nullptr) const;

        void BeginRenderPass(RenderPass *render_pass, Framebuffer *framebuffer, const std::array<float, 4>& color = {0.0f, 0.0f, 0.0f, 1.0f}) const;
        void EndRenderPass() const;
        void BindPipeline(const Pipeline *pipeline) const;
        void BindVertexBuffers(const std::vector<Buffer *> &buffers, const std::vector<VkDeviceSize> &offsets = {0}) const;
        void BindIndexBuffer(Buffer *index_buffer, VkIndexType index_type = VK_INDEX_TYPE_UINT32) const;
        void BindDescriptorSets(const Pipeline *pipeline, uint32_t amount_of_descriptor_sets, VkDescriptorSet *descriptor_sets, uint32_t amount_of_offsets = 0, uint32_t* offsets = nullptr) const;

        template <typename... Args>
        void Draw(Args &&...args) const
        {
            vkCmdDraw(vkCommandBuffer, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void DrawIndexed(Args &&...args) const
        {
            vkCmdDrawIndexed(vkCommandBuffer, std::forward<Args>(args)...);
        }

        void Free() const;

        VkCommandBuffer &GetVkCommandBuffer();

    private:
        const Device& device;
        const CommandPool& commandPool;

        VkCommandBuffer vkCommandBuffer;

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
    };
}