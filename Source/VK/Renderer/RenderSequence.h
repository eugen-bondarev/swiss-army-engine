#ifndef __VK_RenderSequence_h__
#define __VK_RenderSequence_h__

#pragma once

#include "../SwapChain/SwapChain.h"
#include "../Frame/Frame.h"
#include "Renderer.h"

namespace VK
{
    class RenderSequence
    {
    public:
        RenderSequence();
       ~RenderSequence();

        template <typename T, typename... Args>
        T& Emplace(Args&&... args)
        {
            Ptr<T> instance = CreatePtr<T>(std::forward<Args>(args)...);
            T* rawPtr {instance.get()};
            renderers.push_back(std::move(instance));
            return *rawPtr;
        }

        void InitFrames()
        {
            const size_t framesInFlight {3};
            const size_t lastSemaphore {renderers.size()};
            frameManager = CreatePtr<FrameManager>(0, lastSemaphore, lastSemaphore + 1, framesInFlight);
        }

        void Render()
        {
            frameManager->AcquireSwapChainImage();
            
            for (const std::function<void()>& inFrameCallback : inFrame)
            {
                inFrameCallback();
            }

            for (size_t i = 0; i < renderers.size(); ++i)
            {
                const bool lastRenderer {i == renderers.size() - 1};
                renderers[i]->Render(frameManager->GetCurrentFrame(), GetSwapChain().GetCurrentImageIndex(), lastRenderer, i, i + 1);
            }

            frameManager->Present();
        }

        std::vector<std::function<void()>> inFrame;

        Ptr<FrameManager> frameManager;
    private:
        std::vector<Ptr<Renderer>> renderers;

        RenderSequence(const RenderSequence&) = delete;
        RenderSequence& operator=(const RenderSequence&) = delete;
    };
}

#endif