#pragma once

#include "../Objects.h"
#include "../Common.h"

#include "command_pool.h"
#include "../framebuffer/framebuffer.h"
#include "../pipeline/render_pass.h"
#include "../pipeline/pipeline.h"

namespace VK
{
    class Buffer;

    class CommandBuffer
    {
    public:
        CommandBuffer(CommandPool* commandPool, const Global::Device* device = nullptr);
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
        const Global::Device& device;

        CommandPool *commandPool;

        VkCommandBuffer vkCommandBuffer;

        // CommandBuffer(const CommandBuffer &) = delete;
        // CommandBuffer &operator=(const CommandBuffer &) = delete;
    };
}