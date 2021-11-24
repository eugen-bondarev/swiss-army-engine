#ifndef __VK_SwapChain_SwapChain_h__
#define __VK_SwapChain_SwapChain_h__

#pragma once

#include "../../API/SwapChainBase.h"
#include "../Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    FORWARD_DECLARE(Framebuffer);
    FORWARD_DECLARE(RenderPass);
    FORWARD_DECLARE(ImageView);
    FORWARD_DECLARE(Device);

    class SwapChain : public Base::SwapChain
    {
    public:
        SwapChain(RawWindow& window, const Device* device = nullptr);
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

        void InitFramebuffers(const RenderPass& renderPass, const ImageView& depthImageView);

        VK::Framebuffer* GetCurrentScreenFramebuffer();
        std::vector<Ref<VK::Framebuffer>>& GetFramebuffers();

        const std::vector<VkImage>& GetImages() const;
        const std::vector<Ref<ImageView>>& GetImageViews() const;

    private:
        const Device& device;

        std::vector<Ref<VK::Framebuffer>> framebuffers;
        std::vector<Ref<ImageView>> imageViews;
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
        std::vector<VkImage> images;
        VkSwapchainKHR vkSwapChain;
        uint32_t imageIndex{0};
        VkFormat imageFormat;
        VkExtent2D extent;

        VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void CreateImageViews();
    };
}

#endif