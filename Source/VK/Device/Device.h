#ifndef __VK_Device_Device_h__
#define __VK_Device_Device_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Instance);
    FORWARD_DECLARE(GraphicsContext);

    class Device
    {
    public:
        Device(GraphicsContext& graphicsContext, const Instance& instance);
       ~Device();

        inline static const std::vector<const char *> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice) const;
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);

        void CreateLogicalDevice();

        void WaitIdle() const;

        VkPhysicalDevice GetVkPhysicalDevice() const;
        VkDevice GetVkDevice() const;

        const VkPhysicalDeviceProperties& GetProperties() const;

        template <typename T>
        size_t CalculateDynamicAlignment() const
        {
            if (vkProperties.limits.minUniformBufferOffsetAlignment > 0) 
            {
                return (sizeof(T) + vkProperties.limits.minUniformBufferOffsetAlignment - 1) & ~(vkProperties.limits.minUniformBufferOffsetAlignment - 1);
            }

            return sizeof(T);
        }

        // Find supported format for depth buffer.
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
        VkFormat FindDepthFormat() const;
        static bool HasStencilComponent(const VkFormat format);

    private:
        const Instance& instance;
        GraphicsContext& graphicsContext;

        VkPhysicalDevice vkPhysicalDevice;
        VkDevice vkDevice;

        VkPhysicalDeviceProperties vkProperties;

        Device(const Device &) = delete;
        Device &operator=(const Device &) = delete;
    };
}

#endif