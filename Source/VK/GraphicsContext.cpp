#include "GraphicsContext.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        VkApplicationInfo appDesc{};        
        appDesc.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appDesc.pApplicationName = "Hello Triangle";
        appDesc.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appDesc.pEngineName = "No Engine";
        appDesc.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appDesc.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appDesc;

        // auto glfwExtensions = Validation::GetRequestedExtensions();
        std::vector<const char*> glfwExtensions = {};
        createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
        createInfo.ppEnabledExtensionNames = glfwExtensions.data();
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }
}