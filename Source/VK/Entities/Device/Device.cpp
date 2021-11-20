#include "Device.h"

#include "QueueFamily.h"

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

    }

    void Device::PickPhysicalDevice()
    {        
        uint32_t numDevices{0};
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &numDevices, nullptr);

        if (numDevices == 0)
        {
            throw EXCEPTION_WHAT("Failed to find GPUs with Vulkan support.");
        }

        std::vector<VkPhysicalDevice> devices(numDevices);
        vkEnumeratePhysicalDevices(instance.GetVkInstance(), &numDevices, devices.data());

        for (const auto &device : devices)
        {
            if (IsDeviceSuitable(device))
            {
            //     vkPhysicalDevice = device;

            //     vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
            //     Util::Mem::Aligned::minUniformBufferOffsetAlignment = properties.limits.minUniformBufferOffsetAlignment;

            //     break;
            }
        }

        if (vkPhysicalDevice == VK_NULL_HANDLE)
        {
            throw EXCEPTION_WHAT("Failed to find a suitable GPU.");
        }
    }

    void Device::CreateLogicalDevice()
    {

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

        bool extensions_supported{CheckDeviceExtensionSupport(physicalDevice)};
        // bool swap_chain_adequate{false};
        bool swap_chain_adequate{true};

        // if (extensions_supported)
        // {
        //     SupportDetails swap_chain_support_details = QuerySwapChainSupport(device);
        //     swap_chain_adequate = !swap_chain_support_details.formats.empty() && !swap_chain_support_details.presentModes.empty();
        // }

        VkPhysicalDeviceFeatures supported_features;
        vkGetPhysicalDeviceFeatures(physicalDevice, &supported_features);

        return Queues::indices.IsComplete() && swap_chain_adequate && supported_features.samplerAnisotropy;
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