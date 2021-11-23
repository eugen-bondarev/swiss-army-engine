#include "Device.h"

#include "../Instance/Instance.h"
#include "../Instance/Validation.h"
#include "../Surface/Surface.h"
#include "../SwapChain/SupportDetails.h"
#include "QueueFamily.h"
#include <set>

namespace VK
{
    Device::Device(const Instance& instance) : instance{instance}
    {
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

    Device::~Device()
    {
        vkDestroyDevice(vkDevice, nullptr);
    }

    bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void Device::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw EXCEPTION_WHAT("Failed to find GPUs with Vulkan support.");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &deviceCount, devices.data());

        for (const auto &device : devices)
        {
            if (IsDeviceSuitable(device))
            {
                vkPhysicalDevice = device;

                vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
                // Util::Mem::Aligned::minUniformBufferOffsetAlignment = properties.limits.minUniformBufferOffsetAlignment;

                break;
            }
        }

        if (vkPhysicalDevice == VK_NULL_HANDLE)
        {
            throw EXCEPTION_WHAT("Failed to find a suitable GPU.");
        }
    }

    uint32_t Device::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        throw EXCEPTION_WHAT("Failed to find suitable memory type.");

        return 0;
    }

    bool Device::IsDeviceSuitable(VkPhysicalDevice device)
    {
        Queues::indices = Queues::FindQueueFamilies(device);

        bool extensions_supported = CheckDeviceExtensionSupport(device);
        bool swap_chain_adequate = false;

        if (extensions_supported)
        {
            SupportDetails swap_chain_support_details = QuerySwapChainSupport(device);
            swap_chain_adequate = !swap_chain_support_details.formats.empty() && !swap_chain_support_details.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supported_features;
        vkGetPhysicalDeviceFeatures(device, &supported_features);

        return Queues::indices.IsComplete() && swap_chain_adequate && supported_features.samplerAnisotropy;
    }

    void Device::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            Queues::indices.graphicsFamily.value(), Queues::indices.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queueFamily;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queue_create_info);
        }

        VkPhysicalDeviceFeatures device_features{};
        device_features.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queueCreateInfos.data();
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

        create_info.pEnabledFeatures = &device_features;

        create_info.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        create_info.ppEnabledExtensionNames = deviceExtensions.data();

        if (Validation::enableValidationLayers)
        {
            create_info.enabledLayerCount = static_cast<uint32_t>(Validation::validationLayers.size());
            create_info.ppEnabledLayerNames = Validation::validationLayers.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        VK_TRY(vkCreateDevice(vkPhysicalDevice, &create_info, nullptr, &vkDevice));

        vkGetDeviceQueue(vkDevice, Queues::indices.graphicsFamily.value(), 0, &Queues::graphicsQueue);
        vkGetDeviceQueue(vkDevice, Queues::indices.presentFamily.value(), 0, &Queues::presentQueue);
    }

    void Device::WaitIdle() const
    {
        vkDeviceWaitIdle(vkDevice);
    }

    VkPhysicalDevice Device::GetVkPhysicalDevice() const
    {
        return vkPhysicalDevice;
    }

    VkDevice Device::GetVkDevice() const
    {
        return vkDevice;
    }

    
    VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
    {
        for (VkFormat format : candidates) 
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
            {
                return format;
            } 
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
            {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }


    VkFormat Device::FindDepthFormat() const
    {            
        return FindSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }


    bool Device::HasStencilComponent(const VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
}