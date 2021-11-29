#include "RenderTarget.h"

#include "../Device/Device.h"

namespace VK
{
    RenderTarget::RenderTarget(const Vec2ui size, const Vec<Ref<ImageView>>& imageViews, const RenderPass& renderPass, const bool useDepth, const Device& device) : size {size}, renderPass {renderPass}, device {device}
    {
        if (useDepth)
        {
            depthTexture = CreatePtr<Texture2D>(size, device.FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

            for (size_t i = 0; i < imageViews.size(); ++i)
            {
                Ptr<Framebuffer> framebuffer = CreatePtr<Framebuffer>(renderPass, size, *imageViews[i], depthTexture->GetImageView());
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