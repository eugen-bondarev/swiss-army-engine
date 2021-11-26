#ifndef __VK_SPIRV_Reflection_h__
#define __VK_SPIRV_Reflection_h__

#pragma once

#include "../Common.h"

#define SPV_TRY(exp)\
    if (exp != SPV_REFLECT_RESULT_SUCCESS)\
    {\
        throw EXCEPTION_WHAT(#exp);\
    }(void(0))

namespace VK
{
    std::vector<VkDescriptorSetLayoutBinding> ExtractDescriptorSetLayoutBindings(const Util::TextAsset& shaderCode)
    {
        std::vector<VkDescriptorSetLayoutBinding> result;

        SpvReflectShaderModule module;
        SPV_TRY(spvReflectCreateShaderModule(shaderCode.size(), shaderCode.data(), &module));

        uint32_t numBindings {0};
        spvReflectEnumerateDescriptorBindings(&module, &numBindings, nullptr);
        std::vector<SpvReflectDescriptorBinding*> bindings(numBindings);
        spvReflectEnumerateDescriptorBindings(&module, &numBindings, bindings.data());
        result.reserve(numBindings);

        for (size_t i = 0; i < bindings.size(); ++i)
        {
            const uint32_t binding {bindings[i]->binding};

            VkDescriptorSetLayoutBinding vkBinding{};
            vkBinding.binding = binding;        

            if (bindings[i]->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            {
                bool dynamic {false};
                const std::string typeName {bindings[i]->type_description->type_name};
                const size_t typeNameSize {typeName.size()};
                static const std::string suffix {"_dyn"};

                if (typeNameSize > suffix.size())
                {
                    dynamic = typeName.substr(typeNameSize - suffix.size(), typeNameSize) == suffix;
                }

                vkBinding.descriptorType = dynamic ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            }
            else if (bindings[i]->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            {
                vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            }

            result.push_back(std::move(vkBinding));
        }

        spvReflectDestroyShaderModule(&module);

        return result;
    }
}

#endif