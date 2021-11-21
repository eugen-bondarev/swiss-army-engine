#include "Shader.h"

#include "../../GraphicsContext.h"

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
    
    Shader::Shader(const std::string& vsCode, const std::string& fsCode, const Device* device) : device{device ? *device : *GetDevice()}
    {
        vkShaderModules[0] = Util::CreateShaderModule(vsCode, this->device);
        vkShaderModules[1] = Util::CreateShaderModule(fsCode, this->device);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vkShaderModules[0];
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = vkShaderModules[1];
        fragShaderStageInfo.pName = "main";

        vkShaderStages[0] = vertShaderStageInfo;
        vkShaderStages[1] = fragShaderStageInfo;        
    }

    Shader::~Shader()
    {        
        for (const VkShaderModule& module : vkShaderModules)
        {
            vkDestroyShaderModule(device.GetVkDevice(), module, nullptr);
        }
    }
       
    const std::array<VkShaderModule, 2>& Shader::GetVkShaderModule() const
    {
        return vkShaderModules;
    }

    const std::array<VkPipelineShaderStageCreateInfo, 2>& Shader::GetVkShaderStages() const
    {
        return vkShaderStages;
    }
}