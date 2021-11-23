#include "CommandPool.h"

#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
#include "../Device/Device.h"

namespace VK
{
    CommandPool::CommandPool(const Device* device) : device{device ? *device : GetDevice()}
    {
        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = Queues::indices.graphicsFamily.value();
        createInfo.flags = 0;
        VK_TRY(vkCreateCommandPool(this->device.GetVkDevice(), &createInfo, nullptr, &vkCommandPool));        
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(this->device.GetVkDevice(), vkCommandPool, nullptr);        
    }

    void CommandPool::Reset() const
    {
        VK_TRY(vkResetCommandPool(this->device.GetVkDevice(), vkCommandPool, 0));
    }

    const VkCommandPool& CommandPool::GetVkCommandPool() const
    {
        return vkCommandPool;
    }
}