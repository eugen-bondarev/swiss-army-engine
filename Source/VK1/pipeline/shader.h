#pragma once

#include "../Common.h"


namespace VK
{
    FORWARD_DECLARE(Device);
    
    namespace Util
    {
        VkShaderModule CreateShaderModule(const std::string& code, const Device* device = nullptr);
    }

    class Shader
    {
    public:
        Shader(const std::string& vsCode, const std::string& fsCode, const Device* device = nullptr);
        ~Shader();

        const std::array<VkShaderModule, 2>& GetModules() const;
        const std::array<VkPipelineShaderStageCreateInfo, 2>& GetStages() const;

    private:
        const Device& device;

        std::array<VkShaderModule, 2> modules;
        std::array<VkPipelineShaderStageCreateInfo, 2> stages;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
    };
}