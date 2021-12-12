#include "RenderSequence.h"

namespace VK
{
    void RenderSequence::InitFrames()
    {
        const size_t framesInFlight {3};
        const size_t lastSemaphore {renderers.size()};
        frameManager = std::make_unique<FrameManager>(0, lastSemaphore, lastSemaphore + 1, framesInFlight);
    }

    void RenderSequence::Render()
    {
        frameManager->AcquireSwapChainImage();
        
        for (size_t i = 0; i < renderers.size(); ++i)
        {
            const bool lastRenderer {i == renderers.size() - 1};
            renderers[i]->InFrame();
            renderers[i]->Render(frameManager->GetCurrentFrame(), GetSwapChain().GetCurrentImageIndex(), lastRenderer, i, i + 1);
        }

        frameManager->Present();
    }
}