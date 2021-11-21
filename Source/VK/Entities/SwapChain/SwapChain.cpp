#include "SwapChain.h"
#include "SupportDetails.h"
#include "../Device/QueueFamily.h"

#include <algorithm>

#include "../../../Window/RawWindow.h"

namespace VK
{
    VkSurfaceFormatKHR SwapChain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availalbe_formats)
    {				
        VkFormat colorFormat;
        VkColorSpaceKHR colorSpace;
        VkSurfaceFormatKHR format;

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDevice(), surface.GetVkSurface(), &formatCount, NULL);				
        assert(formatCount > 0);

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDevice(), surface.GetVkSurface(), &formatCount, surfaceFormats.data());

        if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            colorSpace = surfaceFormats[0].colorSpace;
        }
        else
        {
            bool found_B8G8R8A8_UNORM = false;
            for (auto&& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    colorFormat = surfaceFormat.format;
                    colorSpace = surfaceFormat.colorSpace;
                    found_B8G8R8A8_UNORM = true;
                    break;
                }
            }

            if (!found_B8G8R8A8_UNORM)
            {
                colorFormat = surfaceFormats[0].format;
                colorSpace = surfaceFormats[0].colorSpace;
            }
        }

        format.colorSpace = colorSpace;
        format.format = colorFormat;

        return format;
    }

    VkPresentModeKHR SwapChain::ChoosePresentMode(const std::vector<VkPresentModeKHR> &available_present_modes)
    {
        for (const auto &availablePresentMode : available_present_modes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseExtent(const unsigned int width, const unsigned int height, const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent =
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void SwapChain::CreateImageViews()
    {
        imageViews.resize(images.size());

        for (size_t i = 0; i < images.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = imageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VK_TRY(vkCreateImageView(device.GetVkDevice(), &createInfo, nullptr, &imageViews[i]));
        }
    }

    void SwapChain::DestroyImageViews()
    {
        for (const VkImageView& imageView : imageViews)
        {
            vkDestroyImageView(device.GetVkDevice(), imageView, nullptr);
        }
    }

    uint32_t SwapChain::AcquireImage(VkSemaphore semaphore)
    {
        vkAcquireNextImageKHR(device.GetVkDevice(), vkSwapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);
        return imageIndex;
    }

    const std::vector<VkImageView>& SwapChain::GetImageViews() const
    {
        return imageViews;
    }

    SwapChain::SwapChain(RawWindow& window, const Surface& surface, const Device& device) : Base::SwapChain(window), surface{surface}, device{device}
    {
        SupportDetails supportDetails = QuerySwapChainSupport(surface, device.GetVkPhysicalDevice());

        surfaceFormat = ChooseSurfaceFormat(supportDetails.formats);
        presentMode = ChoosePresentMode(supportDetails.presentModes);

        // HARD-CODED FOR NOW
        extent = ChooseExtent(window.GetWidth(), window.GetHeight(), supportDetails.capabilities);

        uint32_t numImages = supportDetails.capabilities.minImageCount + 1;

        if (supportDetails.capabilities.maxImageCount > 0 && numImages > supportDetails.capabilities.maxImageCount)
        {
            numImages = supportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface.GetVkSurface();
        createInfo.minImageCount = numImages;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        std::array<uint32_t, 2> queueFamilyIndices =
        {
            Queues::indices.graphicsFamily.value(), Queues::indices.presentFamily.value()
        };

        if (Queues::indices.graphicsFamily != Queues::indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;	  // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_TRY(vkCreateSwapchainKHR(device.GetVkDevice(), &createInfo, nullptr, &vkSwapChain));

        vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &numImages, nullptr);
        images.resize(numImages);
        vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &numImages, images.data());

        imageFormat = surfaceFormat.format;

        CreateImageViews();
    }

    SwapChain::~SwapChain()
    {
        DestroyImageViews();

        vkDestroySwapchainKHR(device.GetVkDevice(), vkSwapChain, nullptr);

        for (auto& framebuffer : framebuffers)
            delete framebuffer;
    }

    void SwapChain::InitFramebuffers(const VkRenderPass& render_pass)
    {
        for (auto& framebuffer : framebuffers)
            delete framebuffer;
        framebuffers.clear();

        // glm::vec2 viewport_size = { extent.width, extent.height };


        for (const VkImageView& image_view : GetImageViews())
        {
            framebuffers.push_back(new Framebuffer(image_view, render_pass, GetWidth(), GetHeight()));
        }
    }

    Framebuffer* SwapChain::GetCurrentScreenFramebuffer()
    {
        return framebuffers[imageIndex];
    }

    std::vector<Framebuffer*>& SwapChain::GetFramebuffers()
    {
        return framebuffers;
    }


    const VkSwapchainKHR& SwapChain::GetVkSwapChain() const
    {
        return vkSwapChain;
    }

    void SwapChain::Present(const unsigned int syncInterval, const unsigned int flags)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        presentInfo.pWaitSemaphores = waitSemaphores.data();

        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &vkSwapChain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        VK_TRY(vkQueuePresentKHR(Queues::presentQueue, &presentInfo));
    }

    void SwapChain::Present1(VkSemaphore* semaphores, const uint32_t numSemaphores)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = numSemaphores;
        presentInfo.pWaitSemaphores = semaphores;

        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &vkSwapChain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        VK_TRY(vkQueuePresentKHR(Queues::presentQueue, &presentInfo));
    }

    void SwapChain::Resize(const unsigned int width, const unsigned int height)
    {

    }
}