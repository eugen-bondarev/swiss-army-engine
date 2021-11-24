#ifndef __VK_SwapChain_SwapChain_h__
#define __VK_SwapChain_SwapChain_h__

#pragma once

#include "../../Graphics/SwapChainBase.h"
#include "../Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Framebuffer);

    class SwapChain : public Base::SwapChain
    {
    public:
        SwapChain(RawWindow& window, const Device* device = nullptr);
       ~SwapChain();

        void Present(const unsigned int syncInterval = 1u, const unsigned int flags = 0u) override;
        void Resize(const unsigned int width = 0u, const unsigned int height = 0u) override;

        uint32_t AcquireImage(VkSemaphore semaphore);

        VkResult Present(const VkSemaphore* waitSemaphores, const uint32_t numWaitSemaphores);

        uint32_t GetCurrentImageIndex() const;

        VkSwapchainKHR GetVkSwapChain() const;
        VkFormat GetImageFormat() const;
        VkSurfaceFormatKHR GetSurfaceFormat() const;
        VkExtent2D GetExtent() const;

        void InitFramebuffers(VkRenderPass& renderPass, const VkImageView& depthImageView);

        VK::Framebuffer* GetCurrentScreenFramebuffer();
        std::vector<Ref<VK::Framebuffer>>& GetFramebuffers();

        const std::vector<VkImage>& GetImages() const;
        const std::vector<VkImageView>& GetImageViews() const;

    private:
        const Device& device;

        std::vector<Ref<VK::Framebuffer>> framebuffers;
        std::vector<VkImageView> imageViews;
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
        void DestroyImageViews();
    };
}

#endif