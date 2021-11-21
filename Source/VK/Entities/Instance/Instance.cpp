#include "Instance.h"

#include "temp_validation.h"

namespace VK
{
    Instance::Instance()
    {        
        if (Validation::enableValidationLayers && !Validation::CheckValidationSupport())
        {
            throw EXCEPTION_WHAT("Validation layers required but not supported.");
        }     

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

        // uint32_t numExtensions{0u};
        // const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&numExtensions);
        // std::vector<const char *> currentExtensions(glfwExtensions, glfwExtensions + numExtensions);
        // currentExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        std::vector<const char*> currentExtensions = Validation::GetRequestedExtensions();

        createInfo.enabledExtensionCount = static_cast<uint32_t>(currentExtensions.size());
        createInfo.ppEnabledExtensionNames = currentExtensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (Validation::enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Validation::validationLayers.size());
            createInfo.ppEnabledLayerNames = Validation::validationLayers.data();

            Validation::PopulateDebugMessengerCreateInfo(debugCreateInfo);
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

        Validation::SetupDebugMessenger(vkInstance);
    }

    Instance::~Instance()
    {        
        Validation::Destroy(vkInstance);
        vkDestroyInstance(vkInstance, nullptr);
    }

    const VkInstance& Instance::GetVkInstance() const
    {
        return vkInstance;
    }
}