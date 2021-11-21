#include "Shader.h"

#include "../../GraphicsContext.h"

namespace VK
{
    namespace Util
    {		
        VkShaderModule CreateShaderModule(const std::string& code, const Device& device)
        {
            VkShaderModuleCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            create_info.codeSize = code.size();
            create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

            VkShaderModule shader_module;
            VK_TRY(vkCreateShaderModule(device.GetVkDevice(), &create_info, nullptr, &shader_module));

            return shader_module;
        }
    }
    
    Shader::Shader(const std::string& vsCode, const std::string& fsCode, const Device* device) : device{device ? *device : *GetDevice()}
    {
        vkShaderModules[0] = Util::CreateShaderModule(vsCode, this->device);
        vkShaderModules[1] = Util::CreateShaderModule(fsCode, this->device);

        VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
        vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_shader_stage_info.module = vkShaderModules[0];
        vert_shader_stage_info.pName = "main";

        VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
        frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_shader_stage_info.module = vkShaderModules[1];
        frag_shader_stage_info.pName = "main";

        vkShaderStages[0] = vert_shader_stage_info;
        vkShaderStages[1] = frag_shader_stage_info;        
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