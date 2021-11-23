#ifndef __VK_Device_Device_h__
#define __VK_Device_Device_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Instance);

    class Device
    {
    public:
        Device(const Instance& instance);
        ~Device();

        const std::vector<const char *> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);

        void CreateLogicalDevice();

        void WaitIdle() const;

        VkPhysicalDevice GetVkPhysicalDevice() const;
        VkDevice GetVkDevice() const;

        VkPhysicalDeviceProperties properties;

        template <typename T>
        size_t CalculateDynamicAlignment()
        {
            if (properties.limits.minUniformBufferOffsetAlignment > 0) 
            {
                return (sizeof(T) + properties.limits.minUniformBufferOffsetAlignment - 1) & ~(properties.limits.minUniformBufferOffsetAlignment - 1);
            }

            return sizeof(T);
        }


        // Find supported format for depth buffer.
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
        VkFormat FindDepthFormat() const;
        static bool HasStencilComponent(const VkFormat format);


    private:
        const Instance& instance;
        VkPhysicalDevice vkPhysicalDevice;
        VkDevice vkDevice;

        Device(const Device &) = delete;
        Device &operator=(const Device &) = delete;
    };
}

#endif