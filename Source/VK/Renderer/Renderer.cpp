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

    void Renderer::CreateUniformBuffers()
    {        
        entityUniformBuffer = CreatePtr<EntityUniformBuffer<EntityUBO>>(50);
        sceneUniformBuffer = CreatePtr<SceneUniformBuffer<SceneUBO>>();
        space = CreatePtr<PerspectiveSpace>(&(*sceneUniformBuffer)());
    }

    Renderer::Renderer(const size_t numCmdBuffers, const size_t samples, const bool useDepth, const bool isOutput, GraphicsContext& ctx) : ctx {ctx}
    {
        CreateCmdEntities(numCmdBuffers);
        CreateUniformBuffers();
        space->SetAspectRatio(ctx.GetWindow().GetAspectRatio());
    }

    SpaceObject& Renderer::Add(const ::Util::ModelAsset& modelAsset, const ::Util::ImageAsset& imageAsset)
    {
        IRenderable* item = new IRenderable(
            modelAsset,
            imageAsset,
            *sceneUniformBuffer,
            *entityUniformBuffer,
            *descriptorSetLayout,
            renderable.size()
        );
        renderable.push_back(Ptr<IRenderable>(item));
        return item->GetSpaceObject();
    }

    void Renderer::RecordAll()
    {
        for (size_t i = 0; i < GetNumCmdBuffers(); ++i)
        {
            Record(i);
        }
    }

    void Renderer::UpdateUniformBuffers(const float ratio)
    {
        (*sceneUniformBuffer).Overwrite();
        (*entityUniformBuffer).Overwrite();
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

    EntityUniformBuffer<EntityUBO>& Renderer::GetEntityUBO()
    {
        return *entityUniformBuffer;
    }

    SceneUniformBuffer<SceneUBO>& Renderer::GetSceneUBO()
    {
        return *sceneUniformBuffer;
    }

    size_t Renderer::GetNumRenderableEntities() const
    {
        return renderable.size();
    }

    SpaceObject& Renderer::GetSpaceObject(const size_t i)
    {
        return renderable[i]->GetSpaceObject();
    }

    PerspectiveSpace& Renderer::GetSpace()
    {
        return *space;
    }
}