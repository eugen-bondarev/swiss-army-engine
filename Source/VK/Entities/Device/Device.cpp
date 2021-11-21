#include "Device.h"

#include "QueueFamily.h"
#include "../SwapChain/SupportDetails.h"

#include <set>

namespace VK
{
    Device::Device(const Instance& instance, const Surface& surface) : instance{instance}, surface{surface}
    {
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

    Device::~Device()
    {
        vkDestroyDevice(vkDevice, nullptr);
    }

    void Device::PickPhysicalDevice()
    {
        uint32_t numDevices{0};
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &numDevices, nullptr);

        if (numDevices == 0)
        {
            throw EXCEPTION_WHAT("Failed to find GPUs with Vulkan support.");
        }

        std::vector<VkPhysicalDevice> physicalDevices(numDevices);
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &numDevices, physicalDevices.data());

        for (const VkPhysicalDevice& physicalDevice : physicalDevices)
        {
            if (IsDeviceSuitable(physicalDevice))
            {
                vkPhysicalDevice = physicalDevice;

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

    void Device::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        const std::set<uint32_t> uniqueQueueFamilies =
        {
            Queues::indices.graphicsFamily.value(), Queues::indices.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (const uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // if (Validation::enableValidationLayers)
        // {
            // create_info.enabledLayerCount = static_cast<uint32_t>(Validation::validationLayers.size());
            // create_info.ppEnabledLayerNames = Validation::validationLayers.data();
        // }
        // else
        // {
            createInfo.enabledLayerCount = 0;
        // }

        VK_TRY(vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkDevice));

        vkGetDeviceQueue(vkDevice, Queues::indices.graphicsFamily.value(), 0, &Queues::graphicsQueue);
        vkGetDeviceQueue(vkDevice, Queues::indices.presentFamily.value(), 0, &Queues::presentQueue);
    }

    bool Device::CheckDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool Device::IsDeviceSuitable(const VkPhysicalDevice& physicalDevice)
    {
        Queues::indices = Queues::FindQueueFamilies(surface, physicalDevice);

        bool extensionsSupported{CheckDeviceExtensionSupport(physicalDevice)};
        bool swapChainAdequate{false};

        if (extensionsSupported)
        {
            SupportDetails swapChainSupportDetails = QuerySwapChainSupport(surface, physicalDevice);
            swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supported_features;
        vkGetPhysicalDeviceFeatures(physicalDevice, &supported_features);

        return Queues::indices.IsComplete() && swapChainAdequate && supported_features.samplerAnisotropy;
    }

    const VkPhysicalDevice& Device::GetVkPhysicalDevice() const
    {
        return vkPhysicalDevice;
    }

    const VkDevice& Device::GetVkDevice() const
    {
        return vkDevice;
    }
}