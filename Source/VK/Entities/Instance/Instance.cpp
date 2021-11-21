#include "Instance.h"

namespace VK
{
    Instance::Instance()
    {        
        VkApplicationInfo appDesc{};        
        appDesc.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appDesc.pApplicationName = "Window";
        appDesc.applicationVersion = VK_MAKE_VERSION(1u, 0u, 0u);
        appDesc.pEngineName = "EngineX";
        appDesc.engineVersion = VK_MAKE_VERSION(1u, 0u, 0u);
        appDesc.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appDesc;

        uint32_t numExtensions{0u};
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&numExtensions);

        createInfo.enabledExtensionCount = numExtensions;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        
        createInfo.enabledLayerCount = 0u;
        createInfo.ppEnabledLayerNames = nullptr;

        VK_TRY(vkCreateInstance(&createInfo, nullptr, &vkInstance));
    }

    Instance::~Instance()
    {        
        vkDestroyInstance(vkInstance, nullptr);
    }

    const VkInstance& Instance::GetVkInstance() const
    {
        return vkInstance;
    }
}