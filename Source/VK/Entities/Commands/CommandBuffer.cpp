#include "CommandBuffer.h"

#include "../../GraphicsContext.h"

namespace VK
{
    CommandBuffer::CommandBuffer(const CommandPool& commandPool, const Device* device) : commandPool{commandPool}, device{device ? *device : *GetDevice()}
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool.GetVkCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_TRY(vkAllocateCommandBuffers(this->device.GetVkDevice(), &allocInfo, &vkCommandBuffer));
    }

    CommandBuffer::~CommandBuffer()
    {
        
    }

    void CommandBuffer::Begin(VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        beginInfo.pInheritanceInfo = nullptr;

        VK_TRY(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo));
    }

    void CommandBuffer::End()
    {
        VK_TRY(vkEndCommandBuffer(vkCommandBuffer));
    }

    void CommandBuffer::SubmitToQueue(const VkQueue& queue, VkSemaphore* wait_semaphore, const VkSemaphore* signal_semaphore, VkFence fence) const
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffer;

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        if (wait_semaphore)
        {
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = wait_semaphore;
            submitInfo.pWaitDstStageMask = &wait_stage;
        }

        if (signal_semaphore)
        {
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signal_semaphore;
        }

        VK_TRY(vkQueueSubmit(queue, 1, &submitInfo, fence));
    }

    void CommandBuffer::BeginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer, const std::array<float, 4>& clearColor) const
    {
        VkRenderPassBeginInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        submit_info.renderPass = renderPass->GetVkRenderPass();
        submit_info.framebuffer = framebuffer->GetVkFramebuffer();
        submit_info.renderArea.extent = {static_cast<uint32_t>(framebuffer->GetWidth()), static_cast<uint32_t>(framebuffer->GetHeight())};
        submit_info.clearValueCount = 1;
        VkClearValue vkClearColor;
        memcpy(&vkClearColor, clearColor.data(), sizeof(VkClearValue));
        submit_info.pClearValues = &vkClearColor;
        
        vkCmdBeginRenderPass(vkCommandBuffer, &submit_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderPass() const
    {
        vkCmdEndRenderPass(vkCommandBuffer);
    }

    void CommandBuffer::BindPipeline(const Pipeline* pipeline) const
    {
        vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());
    }

    void CommandBuffer::BindVertexBuffers(const std::vector<VertexBuffer*>& buffers, const std::vector<VkDeviceSize>& offsets) const
    {
        MY_ASSERT(buffers.size() == offsets.size());

        std::vector<VkBuffer> vkBuffers(buffers.size());
        for (size_t i = 0; i < buffers.size(); ++i)
        {
            vkBuffers[i] = buffers[i]->GetVkBuffer();
        }

        vkCmdBindVertexBuffers(vkCommandBuffer, 0, static_cast<uint32_t>(vkBuffers.size()), vkBuffers.data(), offsets.data());
    }

    void CommandBuffer::BindIndexBuffer(IndexBuffer* indexBuffer, VkIndexType indexType) const
    {
        vkCmdBindIndexBuffer(vkCommandBuffer, indexBuffer->GetVkBuffer(), 0, indexType);
    }

    void CommandBuffer::BindDescriptorSets(const Pipeline* pipeline, uint32_t numDescriptorsSets, VkDescriptorSet* descriptorSets, uint32_t numOffsets, uint32_t* offsets) const
    {
        vkCmdBindDescriptorSets(
            vkCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->GetVkPipelineLayout(), 0,
            numDescriptorsSets, descriptorSets, numOffsets, offsets
        );
    }

    const VkCommandBuffer& CommandBuffer::GetVkCommandBuffer() const
    {
        return vkCommandBuffer;
    }
}