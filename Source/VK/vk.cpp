#include "vk.h"

#include "GraphicsContext.h"

namespace VK
{
    void Bootstrap(GLFWwindow* handle)
    {
        GraphicsContext* ctx = dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext());

        // commandPool = new CommandPool();
        constantInterpolationSampler = new Sampler(VK_FILTER_NEAREST);
        linearInterpolationSampler = new Sampler(VK_FILTER_LINEAR);
    }

    void Shutdown()
    {
        delete constantInterpolationSampler;
        delete linearInterpolationSampler;
        // delete commandPool;
    }
}