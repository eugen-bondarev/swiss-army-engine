#include "Instance.h"

#include "Debug.h"

namespace VK
{
    Instance::Instance(const unsigned int id)
    {
        if (Debug::ValidationLayerEnabled() && !Debug::CheckValidationSupport())
        {
            throw EXCEPTION_WHAT("Validation layers required but not supported.");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto glfwExtensions = Debug::GetRequestedExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
        createInfo.ppEnabledExtensionNames = glfwExtensions.data();

        valid = CreatePtr<Debug>(id);
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (Debug::ValidationLayerEnabled())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Debug::validationLayers.size());
            createInfo.ppEnabledLayerNames = Debug::validationLayers.data();

            valid->PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        VK_TRY(vkCreateInstance(&createInfo, nullptr, &vkInstance));

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        LINE_OUT("Available extensions:");
        for (const auto &ext : extensions)
        {
            LINE_OUT(ext.extensionName);
        }

        valid->SetupDebugMessenger(vkInstance);            
    }

    Instance::~Instance()
    {
        vkDestroyInstance(vkInstance, nullptr);            
    }

    const VkInstance Instance::GetVkInstance() const
    {
        return vkInstance;
    }
}