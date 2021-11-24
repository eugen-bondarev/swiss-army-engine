#ifndef __VK_Instance_Debug_h__
#define __VK_Instance_Debug_h__

#pragma once

#include "../Common.h"

namespace VK
{
    class Debug
    {
    public:
        Debug(const unsigned int id);
       ~Debug();

        static bool ValidationLayerEnabled();
        static bool CheckValidationSupport();
        static std::vector<const char*> GetRequestedExtensions();

        inline static const std::vector<const char*> validationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void SetupDebugMessenger(VkInstance instance);

    private:
        const unsigned int id;
        VkInstance vkInstance;
        VkDebugUtilsMessengerEXT vkDebugMessenger;

        void OutputInfo(const std::string& message) const;
        void OutputWarning(const std::string& message) const;
        void OutputError(const std::string& message) const;

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

        Debug(const Debug&) = delete;
        Debug& operator=(const Debug&) = delete;
    };
}

#endif