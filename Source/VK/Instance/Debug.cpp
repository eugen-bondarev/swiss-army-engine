#include "Debug.h"

namespace VK
{
    Debug::Debug(const unsigned int id) : id{id}
    {
    }

    Debug::~Debug()
    {
        DestroyDebugUtilsMessengerEXT(vkInstance, vkDebugMessenger, nullptr);
    }

    bool Debug::ValidationLayerEnabled()
    {
#ifndef NDEBUG
        return true;
#else
        return false;
#endif
    }

    bool Debug::CheckValidationSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : Debug::validationLayers)
        {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> Debug::GetRequestedExtensions()
    {
        uint32_t glfwExtensionCount{0};
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        return extensions;
    }

    void Debug::OutputInfo(const std::string& message) const
    {
        // LINE_OUT("[Context #" << id << "][Info] " << message << '\n');
    }

    void Debug::OutputWarning(const std::string& message) const
    {
        LINE_OUT("[Context #" << id << "][Warning] " << message << '\n');
    }

    void Debug::OutputError(const std::string& message) const
    {
        LINE_OUT("[Context #" << id << "][Error] " << message << '\n');
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Debug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
    {
        const Debug& self{*static_cast<Debug*>(pUserData)};
        switch (messageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                self.OutputInfo(pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                self.OutputWarning(pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                self.OutputError(pCallbackData->pMessage);
                break;
        }
        return VK_FALSE;
    }

    VkResult Debug::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void Debug::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void Debug::SetupDebugMessenger(VkInstance instance)
    {
        vkInstance = instance;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &vkDebugMessenger) != VK_SUCCESS)
        {
            throw EXCEPTION_WHAT("Failed to set up debug messenger.");
        }
    }

    void Debug::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = this;
    }
}