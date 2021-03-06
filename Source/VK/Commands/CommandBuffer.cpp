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
    CommandBuffer::CommandBuffer(const CommandPool& commandPool, const Device& device) : commandPool{commandPool}, device{device}
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

    void CommandBuffer::Begin(const VkCommandBufferUsageFlags flags) const
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

    void CommandBuffer::SubmitToQueue(const VkQueue& queue, const VkSemaphore* waitSemaphore, const VkSemaphore* signalSemaphore, const VkFence& fence) const
    {
        VkSubmitInfo submitInfo{};
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

    void CommandBuffer::BeginRenderPass(const RenderPass& renderPass, const Framebuffer& framebuffer, const std::array<float, 4>& color) const
    {
        VkRenderPassBeginInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        submitInfo.renderPass = renderPass.GetVkRenderPass();
        submitInfo.framebuffer = framebuffer.GetVkFramebuffer();
        submitInfo.renderArea.extent = {static_cast<uint32_t>(framebuffer.GetSize().x), static_cast<uint32_t>(framebuffer.GetSize().y)};

        VkClearValue vkClearColor;
        memcpy(&vkClearColor, color.data(), sizeof(VkClearValue));
        const VkClearValue vkDepthClearColor{1.0f, 0.0f};
        const std::array<VkClearValue, 2> vkClearValues{vkClearColor, vkDepthClearColor};
        
        submitInfo.clearValueCount = static_cast<uint32_t>(vkClearValues.size());
        submitInfo.pClearValues = vkClearValues.data();
        
        vkCmdBeginRenderPass(vkCommandBuffer, &submitInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderPass() const
    {
        vkCmdEndRenderPass(vkCommandBuffer);
    }

    void CommandBuffer::BindPipeline(const Pipeline& pipeline) const
    {
        vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetVkPipeline());
    }

    void CommandBuffer::BindVertexBuffers(const std::vector<const Buffer*>& buffers, const std::vector<VkDeviceSize>& offsets) const
    {
        VK_ASSERT(buffers.size() == offsets.size());

        std::vector<VkBuffer> vkBuffers(buffers.size());
        for (size_t i = 0; i < buffers.size(); ++i)
        {
            vkBuffers[i] = buffers[i]->GetVkBuffer();
        }

        vkCmdBindVertexBuffers(vkCommandBuffer, 0, static_cast<uint32_t>(vkBuffers.size()), vkBuffers.data(), offsets.data());
    }

    void CommandBuffer::BindIndexBuffer(const Buffer& indexBuffer, const VkIndexType indexType) const
    {
        vkCmdBindIndexBuffer(vkCommandBuffer, indexBuffer.GetVkBuffer(), 0, indexType);
    }

    void CommandBuffer::BindDescriptorSets(const Pipeline& pipeline, const uint32_t numDescriptorSets, const VkDescriptorSet* descriptorSets, const uint32_t numOffsets, const uint32_t* offsets) const
    {
        vkCmdBindDescriptorSets(
            vkCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline.GetVkPipelineLayout(), 
            0,
            numDescriptorSets, 
            descriptorSets, 
            numOffsets, 
            offsets
        );
    }

    void CommandBuffer::SetViewport(const float width, const float height, const float x, const float y, const float minDepth, const float maxDepth)
    {
        VkViewport viewport{};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;
        vkCmdSetViewport(GetVkCommandBuffer(), 0, 1, &viewport);
    }

    void CommandBuffer::SetScissors(const float width, const float height, const float offsetX, const float offsetY)
    {
        VkRect2D scissor{};
        scissor.offset = {static_cast<int32_t>(offsetX), static_cast<int32_t>(offsetY)};
        scissor.extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        vkCmdSetScissor(GetVkCommandBuffer(), 0, 1, &scissor);
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