#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(Framebuffer);

    class SwapChain
    {
    public:
        SwapChain(GLFWwindow* handle, const Device* device = nullptr);
        ~SwapChain();

        uint32_t AcquireImage(VkSemaphore semaphore);

        VkResult Present(VkSemaphore* wait_semaphores, uint32_t wait_semaphore_count);

        uint32_t GetCurrentImageIndex() const;

        VkSwapchainKHR GetVkSwapChain() const;
        VkFormat GetImageFormat() const;
        VkSurfaceFormatKHR GetSurfaceFormat() const;
        VkExtent2D GetExtent() const;

        void InitFramebuffers(VkRenderPass& render_pass, const VkImageView& depthImageView);
        VK::Framebuffer* GetCurrentScreenFramebuffer();
        std::vector<VK::Framebuffer*>& GetFramebuffers();

        const std::vector<VkImage> &GetImages() const;
        const std::vector<VkImageView> &GetImageViews() const;

    private:
        const Device& device;

        GLFWwindow* handle;

        uint32_t imageIndex { 0 };

        std::vector<VK::Framebuffer*> framebuffers;

        VkSwapchainKHR vkSwapChain;

        VkFormat imageFormat;
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void CreateImageViews();
        void DestroyImageViews();
    };
}