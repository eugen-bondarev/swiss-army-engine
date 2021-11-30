#include "SwapChain.h"

#include "../RenderTarget/RenderTarget.h"
#include "../Framebuffer/Framebuffer.h"
#include "../Pipeline/RenderPass.h"
#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
#include "../Surface/Surface.h"
#include "../Image/ImageView.h"
#include "../Device/Device.h"
#include "SupportDetails.h"
#include <algorithm>

namespace VK
{
    SwapChain::SwapChain(RawWindow& window, const Device& device) : Base::SwapChain(window), device{device}
    {
        SupportDetails supportDetails = QuerySwapChainSupport(this->device.GetVkPhysicalDevice());

        // surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
        surfaceFormat = ChooseSurfaceFormat(supportDetails.formats);
        presentMode = ChoosePresentMode(supportDetails.presentModes);
        extent = ChooseExtent(supportDetails.capabilities);

        uint32_t imageCount {supportDetails.capabilities.minImageCount + 1};

        if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount)
        {
            imageCount = supportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = GetSurface().GetVkSurface();
        createInfo.minImageCount = imageCount;
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
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_TRY(vkCreateSwapchainKHR(device.GetVkDevice(), &createInfo, nullptr, &vkSwapChain));

        vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &imageCount, images.data());

        imageFormat = surfaceFormat.format;

        CreateImageViews();
    }

    SwapChain::~SwapChain()
    {
        vkDestroySwapchainKHR(device.GetVkDevice(), vkSwapChain, nullptr);
    }

    void SwapChain::Present(const unsigned int syncInterval, const unsigned int flags)
    {

    }

    void SwapChain::Resize(const Vec2ui size)
    {

    }

    uint32_t SwapChain::AcquireImage(VkSemaphore semaphore)
    {
        VkResult result = vkAcquireNextImageKHR(device.GetVkDevice(), vkSwapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {

        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image.");
        }

        return imageIndex;
    }

    VkResult SwapChain::Present(const VkSemaphore* waitSemaphores, const uint32_t numWaitSemaphores)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = numWaitSemaphores;
        presentInfo.pWaitSemaphores = waitSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &vkSwapChain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;
        return vkQueuePresentKHR(Queues::presentQueue, &presentInfo);
    }

    uint32_t SwapChain::GetCurrentImageIndex() const
    {
        return imageIndex;
    }

    VkSurfaceFormatKHR SwapChain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availalbe_formats)
    {				
        VkFormat colorFormat;
        VkColorSpaceKHR colorSpace;
        VkSurfaceFormatKHR format;

        // Get list of supported surface formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDevice(), GetSurface().GetVkSurface(), &formatCount, NULL);				
        VK_ASSERT(formatCount > 0);

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDevice(), GetSurface().GetVkSurface(), &formatCount, surfaceFormats.data());

        // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
        // there is no preferred format, so we assume VK_FORMAT_B8G8R8A8_UNORM
        if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            colorSpace = surfaceFormats[0].colorSpace;
        }
        else
        {
            // iterate over the list of available surface format and
            // check for the presence of VK_FORMAT_B8G8R8A8_UNORM
            bool B8G8R8A8UNORMFound = false;
            for (const auto& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    colorFormat = surfaceFormat.format;
                    colorSpace = surfaceFormat.colorSpace;
                    B8G8R8A8UNORMFound = true;
                    break;
                }
            }

            // in case VK_FORMAT_B8G8R8A8_UNORM is not available
            // select the first available color format
            if (!B8G8R8A8UNORMFound)
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
        for (const auto &available_present_mode : available_present_modes)
        {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return available_present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(window.GetHandle(), &width, &height);
            VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void SwapChain::CreateImageViews()
    {
        std::vector<VkImageView> vkImageViews;
        vkImageViews.resize(images.size());

        for (size_t i = 0; i < images.size(); ++i)
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
            VK_TRY(vkCreateImageView(device.GetVkDevice(), &createInfo, nullptr, &vkImageViews[i]));

            imageViews.push_back(CreateRef<ImageView>(vkImageViews[i], device));
        }
    }

    const Vec<Ref<ImageView>>& SwapChain::GetImageViews() const
    {
        return imageViews;
    }

    VkSwapchainKHR SwapChain::GetVkSwapChain() const
    {
        return vkSwapChain;
    }

    VkFormat SwapChain::GetImageFormat() const
    {
        return imageFormat;
    }

    VkSurfaceFormatKHR SwapChain::GetSurfaceFormat() const
    {
        return surfaceFormat;
    }

    VkExtent2D SwapChain::GetExtent() const
    {
        return extent;
    }

    VkAttachmentDescription SwapChain::GetDefaultAttachmentDescription(const VkSampleCountFlagBits samples) const
    {
        return Util::CreateAttachment(
            GetImageFormat(), 
            VK_IMAGE_LAYOUT_UNDEFINED, 
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
            VK_ATTACHMENT_LOAD_OP_CLEAR, 
            VK_ATTACHMENT_STORE_OP_STORE, 
            VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
            VK_ATTACHMENT_STORE_OP_DONT_CARE, 
            samples
        );
    }

    size_t SwapChain::GetNumBuffers() const
    {
        return imageViews.size();
    }
}