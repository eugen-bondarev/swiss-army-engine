#include "CommandBuffer.h"

#include "../Framebuffer/Framebuffer.h"
#include "../Pipeline/RenderPass.h"
#include "../Pipeline/Pipeline.h"
#include "../GraphicsContext.h"
#include "../Device/Device.h"
#include "../Memory/Buffer.h"
#include "CommandPool.h"

namespace VK
{
    CommandBuffer::CommandBuffer(const CommandPool* commandPool, const Device* device) : commandPool{*commandPool}, device{device ?* device : GetDevice()}
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = this->commandPool.GetVkCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_TRY(vkAllocateCommandBuffers(this->device.GetVkDevice(), &allocInfo, &vkCommandBuffer));        
    }

    CommandBuffer::~CommandBuffer()
    {
        Free();
    }

    void CommandBuffer::Begin(VkCommandBufferUsageFlags flags) const
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        beginInfo.pInheritanceInfo = nullptr;
        VK_TRY(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo));
    }

    void CommandBuffer::End() const
    {
        VK_TRY(vkEndCommandBuffer(vkCommandBuffer));
    }

    void CommandBuffer::SubmitToQueue(const VkQueue& queue, const VkSemaphore* waitSemaphore, const VkSemaphore* signalSemaphore, VkFence fence) const
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffer;

        const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        if (waitSemaphore)
        {
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphore;
            submitInfo.pWaitDstStageMask = &waitStage;
        }

        if (signalSemaphore)
        {
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphore;
        }

        VK_TRY(vkQueueSubmit(queue, 1, &submitInfo, fence));
    }

    void CommandBuffer::BeginRenderPass(const RenderPass& renderPass, Framebuffer* framebuffer, const std::array<float, 4>& color) const
    {
        VkRenderPassBeginInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        submitInfo.renderPass = renderPass.GetVkRenderPass();
        submitInfo.framebuffer = framebuffer->GetVkFramebuffer();
        submitInfo.renderArea.extent = {static_cast<uint32_t>(framebuffer->GetSize().x), static_cast<uint32_t>(framebuffer->GetSize().y)};

        VkClearValue clear_color;
        memcpy(&clear_color, color.data(), sizeof(VkClearValue));

        std::array<VkClearValue, 2> clearValues{clear_color, { 1.0f, 0.0f }};
        
        submitInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        submitInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(vkCommandBuffer, &submitInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderPass() const
    {
        vkCmdEndRenderPass(vkCommandBuffer);
    }

    void CommandBuffer::BindPipeline(const Pipeline* pipeline) const
    {
        vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());
    }

    void CommandBuffer::BindVertexBuffers(const std::vector<Buffer*>& buffers, const std::vector<VkDeviceSize>& offsets) const
    {
        VK_ASSERT(buffers.size() == offsets.size());

        std::vector<VkBuffer> vkBuffers(buffers.size());
        for (size_t i = 0; i < buffers.size(); ++i)
        {
            vkBuffers[i] = buffers[i]->GetVkBuffer();
        }

        vkCmdBindVertexBuffers(vkCommandBuffer, 0, static_cast<uint32_t>(vkBuffers.size()), vkBuffers.data(), offsets.data());
    }

    void CommandBuffer::BindIndexBuffer(Buffer* index_buffer, VkIndexType index_type) const
    {
        vkCmdBindIndexBuffer(vkCommandBuffer, index_buffer->GetVkBuffer(), 0, index_type);
    }

    void CommandBuffer::BindDescriptorSets(const Pipeline* pipeline, uint32_t numDescriptorSets, const VkDescriptorSet* descriptorSets, uint32_t numOffsets, uint32_t* offsets) const
    {
        vkCmdBindDescriptorSets(
            vkCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->GetVkPipelineLayout(), 
            0,
            numDescriptorSets, 
            descriptorSets, 
            numOffsets, 
            offsets
        );
    }

    void CommandBuffer::Free() const
    {
        vkFreeCommandBuffers(device.GetVkDevice(), commandPool.GetVkCommandPool(), 1, &vkCommandBuffer);
    }

    const VkCommandBuffer& CommandBuffer::GetVkCommandBuffer() const
    {
        return vkCommandBuffer;
    }
}