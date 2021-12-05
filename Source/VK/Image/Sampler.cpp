#include "sampler.h"

#include "../Device/Device.h"

namespace VK
{
    Sampler::Sampler(VkFilter filter, const Device& device) : device{device}
    {
        VkSamplerCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.magFilter = filter;
        createInfo.minFilter = filter;
        createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.anisotropyEnable = VK_TRUE;
        createInfo.maxAnisotropy = this->device.GetProperties().limits.maxSamplerAnisotropy;
        createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        createInfo.unnormalizedCoordinates = VK_FALSE;

        createInfo.compareEnable = VK_FALSE;
        createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        createInfo.minLod = 0.0f;
        createInfo.maxLod = 10.0f;
        createInfo.mipLodBias = 0.0f;

        VK_TRY(vkCreateSampler(this->device.GetVkDevice(), &createInfo, nullptr, &vkSampler));
    }

    Sampler::~Sampler()
    {
        vkDestroySampler(device.GetVkDevice(), vkSampler, nullptr);
    }

    const VkSampler& Sampler::GetVkSampler() const
    {
        return vkSampler;
    }
}