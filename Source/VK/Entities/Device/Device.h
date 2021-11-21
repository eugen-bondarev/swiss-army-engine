#ifndef __VK_Entities_Device_Device_h__
#define __VK_Entities_Device_Device_h__

#include "../../Common.h"
#include "../Instance/Instance.h"
#include "../Surface/Surface.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    class Device
    {
    public:
        Device(const Instance& instance, const Surface& surface);
       ~Device();

        const std::vector<const char *> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const VkPhysicalDevice& GetVkPhysicalDevice() const;
        const VkDevice& GetVkDevice() const;

    private:
        const Instance& instance;
        const Surface& surface;
        VkPhysicalDevice vkPhysicalDevice{VK_NULL_HANDLE};
        VkDevice vkDevice{VK_NULL_HANDLE};

        VkPhysicalDeviceProperties properties;

        bool CheckDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice);
        bool IsDeviceSuitable(const VkPhysicalDevice& physicalDevice);

        void PickPhysicalDevice();
        void CreateLogicalDevice();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
    };
}

#endif