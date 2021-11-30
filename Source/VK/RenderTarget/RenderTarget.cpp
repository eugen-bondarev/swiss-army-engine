#include "RenderTarget.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/Device.h"

namespace VK
{
    RenderTarget::RenderTarget(const Vec2ui size, const Vec<Ref<ImageView>>& imageViews, const RenderPass& renderPass, const size_t samples, const bool useDepth, const Device& device) : size {size}, renderPass {renderPass}, device {device}
    {
        // const bool useDepth {static_cast<bool>(rendererFlags & RendererFlags_Depth)};
        const bool useMultisample {samples > 0};

        if (useDepth)
        {
            depthTexture = CreatePtr<Texture2D>(size, device.FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, useMultisample ? VK_SAMPLE_COUNT_8_BIT : VK_SAMPLE_COUNT_1_BIT);
        }

        if (useMultisample)
        {
            multiSampleTexture = CreatePtr<Texture2D>(size, GetSwapChain().GetImageFormat(), VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_SAMPLE_COUNT_8_BIT);
        }

        const ImageView* depthImageView {depthTexture ? &depthTexture->GetImageView() : nullptr};
        const ImageView* multisamplerImageView {multiSampleTexture ? &multiSampleTexture->GetImageView() : nullptr};

        for (size_t i = 0; i < imageViews.size(); ++i)
        {
            Ptr<Framebuffer> framebuffer = CreatePtr<Framebuffer>(renderPass, size, *imageViews[i], depthImageView, multisamplerImageView);
            framebuffers.push_back(std::move(framebuffer));
        }
    }

    const Framebuffer& RenderTarget::GetFramebuffer(const size_t i) const
    {
        return *framebuffers[i];
    }
}