#ifndef __VK_Entities_Pipeline_Shader_h__
#define __VK_Entities_Pipeline_Shader_h__

#pragma once

#include "../../Common.h"
#include "../Device/Device.h"

namespace VK
{
    namespace Util
    {
        VkShaderModule CreateShaderModule(const std::string& code, const Device& device);
    }

    class Shader
    {
    public:
        Shader(const std::string& vsCode, const std::string& fsCode, const Device* device = nullptr);
       ~Shader();
       
        const std::array<VkShaderModule, 2>& GetVkShaderModule() const;
        const std::array<VkPipelineShaderStageCreateInfo, 2>& GetVkShaderStages() const;

    private:
        const Device& device;

        std::array<VkShaderModule, 2> vkShaderModules;
        std::array<VkPipelineShaderStageCreateInfo, 2> vkShaderStages;
    
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
    };
}

#endif