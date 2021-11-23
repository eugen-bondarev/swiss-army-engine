#include "shader.h"

#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{
    namespace Util
    {		
        VkShaderModule CreateShaderModule(const std::string& code, const Device* device)
        {
            VkShaderModuleCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            create_info.codeSize = code.size();
            create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

            VkShaderModule shader_module;
            VK_TRY(vkCreateShaderModule(device->GetVkDevice(), &create_info, nullptr, &shader_module));

            return shader_module;
        }
    }

    Shader::Shader(const std::string& vs_code, const std::string& fs_code, const Device* device) : device{device ? *device : GetDevice()}
    {
        modules[0] = Util::CreateShaderModule(vs_code, &this->device);
        modules[1] = Util::CreateShaderModule(fs_code, &this->device);

        VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
        vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_shader_stage_info.module = modules[0];
        vert_shader_stage_info.pName = "main";

        VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
        frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_shader_stage_info.module = modules[1];
        frag_shader_stage_info.pName = "main";

        stages[0] = vert_shader_stage_info;
        stages[1] = frag_shader_stage_info;
    }

    Shader::~Shader()
    {
        for (const auto& module : modules)
            vkDestroyShaderModule(device.GetVkDevice(), module, nullptr);

        
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