#include "CommandBuffer.h"

#include "../../GraphicsContext.h"

namespace VK
{
    CommandBuffer::CommandBuffer(const CommandPool& commandPool, const Device* device) : commandPool{commandPool}, device{device ? *device : *GetDevice()}
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool.GetVkCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_TRY(vkAllocateCommandBuffers(this->device.GetVkDevice(), &allocInfo, &vkCommandBuffer));
    }

    CommandBuffer::~CommandBuffer()
    {
        
    }

    const VkCommandBuffer& CommandBuffer::GetVkCommandBuffer() const
    {
        return vkCommandBuffer;
    }
}