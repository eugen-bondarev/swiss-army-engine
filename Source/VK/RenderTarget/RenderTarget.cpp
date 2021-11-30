#include "RenderTarget.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/Device.h"

namespace VK
{
    RenderTarget::RenderTarget(const Vec2ui size, const Vec<Ref<ImageView>>& imageViews, const RenderPass& renderPass, const bool useDepth, const Device& device) : size {size}, renderPass {renderPass}, device {device}
    {
        multiSampleTexture = CreatePtr<Texture2D>(size, GetSwapChain().GetImageFormat(), VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_SAMPLE_COUNT_8_BIT);

        if (useDepth)
        {
            depthTexture = CreatePtr<Texture2D>(size, device.FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, VK_SAMPLE_COUNT_8_BIT);

            for (size_t i = 0; i < imageViews.size(); ++i)
            {
                Ptr<Framebuffer> framebuffer = CreatePtr<Framebuffer>(renderPass, size, *imageViews[i], depthTexture->GetImageView(), multiSampleTexture->GetImageView());
                framebuffers.push_back(std::move(framebuffer));
            }
        }
        else
        {

        }
    }

    const Framebuffer& RenderTarget::GetFramebuffer(const size_t i) const
    {
        return *framebuffers[i];
    }
}