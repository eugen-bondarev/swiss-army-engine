#ifndef __VK_RenderTarget_RenderTarget_h__
#define __VK_RenderTarget_RenderTarget_h__

#include "../Framebuffer/Framebuffer.h"
#include "../Pipeline/RenderPass.h"
#include "../Image/Texture2D.h"
#include "../Image/ImageView.h"
#include "../GraphicsContext.h"

namespace VK
{
    FORWARD_DECLARE(Device);

    class RenderTarget
    {
    public:
        RenderTarget(const Vec2ui size, const Vec<Ref<ImageView>>& imageViews, const RenderPass& renderPass, const bool useDepth = true, const Device& device = GetDevice());

        const Framebuffer& GetFramebuffer(const size_t i) const;
        
    private:
        const Device& device;
        Vec<Ptr<Framebuffer>> framebuffers;
        Ptr<Texture2D> depthTexture;
        Vec2ui size;
        const RenderPass& renderPass;
    };
}

#endif