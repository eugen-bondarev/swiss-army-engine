#include "vk.h"

#include "GraphicsContext.h"

namespace VK
{
    void Bootstrap(GLFWwindow* handle)
    {
        GraphicsContext* ctx = dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext());

        Global::commandPool = new CommandPool();
        Global::constantInterpolationSampler = new Sampler(VK_FILTER_NEAREST);
        Global::linearInterpolationSampler = new Sampler(VK_FILTER_LINEAR);

        // Not the best idea.
        Global::descriptorPool = new DescriptorPool({				
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        });
    }

    void Shutdown()
    {
        delete Global::descriptorPool;
        delete Global::constantInterpolationSampler;
        delete Global::linearInterpolationSampler;
        delete Global::commandPool;
    }
}