#ifndef __VK_Entities_SwapChain_SwapChain_h__
#define __VK_Entities_SwapChain_SwapChain_h__

#pragma once

#include "../../Common.h"
#include "../Surface/Surface.h"
#include "../Device/Device.h"
#include "../Framebuffer/Framebuffer.h"
#include "../../../Graphics/SwapChainBase.h"

namespace VK
{
    class SwapChain : public Base::SwapChain
    {
    public:
        SwapChain(RawWindow& window, const Surface& surface, const Device& device);
       ~SwapChain();

        // Temp
        std::vector<VkSemaphore> waitSemaphores;
        uint32_t imageIndex{0};

        uint32_t AcquireImage(VkSemaphore semaphore);

        void Present(const unsigned int syncInterval, const unsigned int flags) override;
        void Present1(VkSemaphore* semaphores, const uint32_t numSemaphores);
        void Resize(const unsigned int width, const unsigned int height) override;

        const std::vector<VkImageView>& GetImageViews() const;

        const VkSwapchainKHR& GetVkSwapChain() const;

        void InitFramebuffers(const VkRenderPass& render_pass);
        Framebuffer* GetCurrentScreenFramebuffer();
        std::vector<Framebuffer*>& GetFramebuffers();

    private:
        const Surface& surface;
        const Device& device;

        std::vector<Framebuffer*> framebuffers;

        VkSwapchainKHR vkSwapChain;

        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkFormat imageFormat;

        VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availalbe_formats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &available_present_modes);
        VkExtent2D ChooseExtent(const unsigned int width, const unsigned int height, const VkSurfaceCapabilitiesKHR &capabilities);

        void CreateImageViews();
        void DestroyImageViews();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;
    };
}

#endif