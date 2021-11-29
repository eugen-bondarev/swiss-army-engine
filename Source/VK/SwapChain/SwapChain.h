#ifndef __VK_SwapChain_SwapChain_h__
#define __VK_SwapChain_SwapChain_h__

#pragma once

#include "../../API/SwapChainBase.h"
#include "../GraphicsContext.h"
#include "../Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    FORWARD_DECLARE(RenderTarget);
    FORWARD_DECLARE(Framebuffer);
    FORWARD_DECLARE(RenderPass);
    FORWARD_DECLARE(ImageView);
    FORWARD_DECLARE(Texture2D);
    FORWARD_DECLARE(Device);

    class SwapChain : public Base::SwapChain
    {
    public:
        SwapChain(RawWindow& window, const Device& device = GetDevice());
       ~SwapChain();

        void Present(const unsigned int syncInterval = 1u, const unsigned int flags = 0u) override;
        void Resize(const Vec2ui size) override;

        uint32_t AcquireImage(VkSemaphore semaphore);

        VkResult Present(const VkSemaphore* waitSemaphores, const uint32_t numWaitSemaphores);

        uint32_t GetCurrentImageIndex() const;

        VkSwapchainKHR GetVkSwapChain() const;
        VkFormat GetImageFormat() const;
        VkSurfaceFormatKHR GetSurfaceFormat() const;
        VkExtent2D GetExtent() const;

        size_t GetNumBuffers() const;

        const Vec<Ref<ImageView>>& GetImageViews() const;

        VkAttachmentDescription GetDefaultAttachmentDescription() const;
        void CreateImageViews();

    private:
        const Device& device;

        VkSwapchainKHR vkSwapChain;

        std::vector<Ref<VK::Framebuffer>> framebuffers;
        std::vector<Ref<ImageView>> imageViews;
        std::vector<VkImage> images;

        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;
        
        uint32_t imageIndex{0};
        VkFormat imageFormat;

        VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    };
}

#endif