#include "CommandPool.h"

#include "../Device/QueueFamily.h"

#include "../../GraphicsContext.h"

namespace VK
{
    CommandPool::CommandPool() : device{*GetDevice()}
    {
        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = Queues::indices.graphicsFamily.value();
        createInfo.flags = 0;

        VK_TRY(vkCreateCommandPool(device.GetVkDevice(), &createInfo, nullptr, &vkCommandPool));
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(device.GetVkDevice(), vkCommandPool, nullptr);        
    }

    void CommandPool::Reset()
    {
        VK_TRY(vkResetCommandPool(device.GetVkDevice(), vkCommandPool, 0));
    }

    const VkCommandPool& CommandPool::GetVkCommandPool() const
    {
        return vkCommandPool;
    }
}