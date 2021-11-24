#include "shader.h"

#include "../GraphicsContext.h"
#include "../Device/Device.h"

namespace VK
{
    namespace Util
    {		
        VkShaderModule CreateShaderModule(const std::string& code, const Device& device)
        {
            VkShaderModuleCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = code.size();
            createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

            VkShaderModule shaderModule;
            VK_TRY(vkCreateShaderModule(device.GetVkDevice(), &createInfo, nullptr, &shaderModule));

            return shaderModule;
        }
    }

    Shader::Shader(const std::string& vsCode, const std::string& fsCode, const Device& device) : device{device}
    {
        modules[0] = Util::CreateShaderModule(vsCode, device);
        modules[1] = Util::CreateShaderModule(fsCode, device);

        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo{};
        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageCreateInfo.module = modules[0];
        vertShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo{};
        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageCreateInfo.module = modules[1];
        fragShaderStageCreateInfo.pName = "main";

        stages[0] = vertShaderStageCreateInfo;
        stages[1] = fragShaderStageCreateInfo;
    }

    Shader::~Shader()
    {
        for (const auto& module : modules)
        {
            vkDestroyShaderModule(device.GetVkDevice(), module, nullptr);
        }        
    }

    const std::array<VkShaderModule, 2>& Shader::GetModules() const
    {
        return modules;
    }

    const std::array<VkPipelineShaderStageCreateInfo, 2>& Shader::GetStages() const
    {
        return stages;
    }	
}