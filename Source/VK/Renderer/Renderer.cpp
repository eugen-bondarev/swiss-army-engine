#include "Renderer.h"

#include "../Pipeline/VertexLayouts/Layouts.h"
#include "../RenderTarget/RenderTarget.h"
#include "../Descriptors/DescriptorSet.h"
#include "../SwapChain/SwapChain.h"
#include <gtc/matrix_transform.hpp>
#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
#include "../Device/Device.h"

namespace VK
{    
    VkImageLayout FlagsToFinalImageLayout(const RendererFlags flags)
    {
        VK_ASSERT(!(flags & RendererFlags_Output && flags & RendererFlags_Offscreen));
        if (flags & RendererFlags_Output) return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        if (flags & RendererFlags_Offscreen) return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    VkImageLayout FlagsToInitialImageLayout(const RendererFlags flags)
    {
        VK_ASSERT(!(flags & RendererFlags_Load && flags & RendererFlags_Clear));
        if (flags & RendererFlags_Load) return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        if (flags & RendererFlags_Clear) return VK_IMAGE_LAYOUT_UNDEFINED;
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }

    VkAttachmentLoadOp FlagsToLoadOp(const RendererFlags flags)
    {
        if (flags & RendererFlags_Load) return VK_ATTACHMENT_LOAD_OP_LOAD;
        if (flags & RendererFlags_Clear) return VK_ATTACHMENT_LOAD_OP_CLEAR;
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }

    void Renderer::CreateCmdEntities(const size_t numCmdBuffers)
    {
        commandBuffers.reserve(numCmdBuffers);
        commandPools.reserve(numCmdBuffers);
        for (size_t i = 0; i < numCmdBuffers; ++i)
        {
            Ptr<CommandPool> pool = CreatePtr<CommandPool>();
            commandBuffers.push_back(CreatePtr<CommandBuffer>(*pool));
            commandPools.push_back(std::move(pool));
        }
    }

    Renderer::Renderer(const size_t numCmdBuffers, const size_t samples, const RendererFlags flags, GraphicsContext& ctx) 
        : ctx {ctx}, samples {samples}, flags {flags}
    {
        CreateCmdEntities(numCmdBuffers);

        const Vec2f halfSize {Math::CastTo<Vec2f>(ctx.GetWindow().GetSize()) / 2.0f};
        // orthogonalSpace->Set(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);
    }

    void Renderer::RecordAll()
    {
        for (size_t i = 0; i < GetNumCmdBuffers(); ++i)
        {
            Record(i);
        }
    }

    void Renderer::Render(const Frame& frame, const uint32_t swapChainImageIndex, const bool resetFence, const uint32_t waitSemaphoreIndex, const uint32_t signalSemaphoreIndex)
    {
        // Todo: Do smth with sync-objects.
        const VkFence& fence = frame.GetInFlightFence();
        const VkSemaphore& wait = frame.GetSemaphore(waitSemaphoreIndex);
        const VkSemaphore& signal = frame.GetSemaphore(signalSemaphoreIndex);

        const CommandBuffer& cmd = *commandBuffers[swapChainImageIndex];
        if (resetFence)
        {
            vkResetFences(ctx.GetDevice().GetVkDevice(), 1, &fence);
            cmd.SubmitToQueue(Queues::graphicsQueue, &wait, &signal, fence);
        }
        else
        {
            cmd.SubmitToQueue(Queues::graphicsQueue, &wait, &signal);
        }
    }

    CommandBuffer& Renderer::GetCommandBuffer(const size_t i)
    {
        return *commandBuffers[i];
    }

    CommandPool& Renderer::GetCommandPool(const size_t i)
    {
        return *commandPools[i];
    }

    size_t Renderer::GetNumCmdBuffers() const
    {
        return commandBuffers.size();
    }

    size_t Renderer::GetNumCmdPool() const
    {
        return commandPools.size();
    }
}