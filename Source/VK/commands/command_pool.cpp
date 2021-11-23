#include "command_pool.h"

#include "../device/queue_family.h"
#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{
    namespace Global
    {
        CommandPool *commandPool;
    }

    CommandPool::CommandPool(const Global::Device* device) : device{device ? *device : GetDevice()}
    {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = Global::Queues::indices.graphicsFamily.value();
        pool_info.flags = 0; // Optional

        VK_TRY(vkCreateCommandPool(this->device.GetVkDevice(), &pool_info, nullptr, &vkCommandPool));        
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(this->device.GetVkDevice(), vkCommandPool, nullptr);

        
    }

    void CommandPool::Reset() const
    {
        VK_TRY(vkResetCommandPool(this->device.GetVkDevice(), vkCommandPool, 0));
    }

    VkCommandPool CommandPool::GetVkCommandPool() const
    {
        return vkCommandPool;
    }
}