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
        RenderSequence() = default;

        void InitFrames();
        void Render();

        template <typename T, typename... Args>
        T& Emplace(Args&&... args)
        {
            std::unique_ptr<T> instance = std::make_unique<T>(std::forward<Args>(args)...);
            T* rawPtr {instance.get()};
            renderers.push_back(std::move(instance));
            return *rawPtr;
        }

    private:
        std::unique_ptr<FrameManager> frameManager;
        std::vector<std::unique_ptr<Renderer>> renderers;

        RenderSequence(const RenderSequence&) = delete;
        RenderSequence& operator=(const RenderSequence&) = delete;
    };
}

#endif