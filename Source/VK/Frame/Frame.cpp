#include "frame.h"

#include "../device/device.h"
#include "../SwapChain/SwapChain.h"

#include "../GraphicsContext.h"

namespace VK
{
    Frame::Frame(const uint32_t firstSemaphore, const uint32_t lastSemaphore, const uint32_t numSemaphores, const Device* device) : device{device ? *device : GetDevice()}, firstSemaphore{firstSemaphore}, lastSemaphore{lastSemaphore}
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        semaphores.resize(numSemaphores);

        for (size_t i = 0; i < numSemaphores; ++i)
        {
            VK_TRY(vkCreateSemaphore(this->device.GetVkDevice(), &semaphoreCreateInfo, nullptr, &semaphores[i]));
        }
        
        VK_TRY(vkCreateFence(this->device.GetVkDevice(), &fenceCreateInfo, nullptr, &inFlightFence));
    }

    Frame::~Frame()
    {
        for (const VkSemaphore& semaphore : semaphores)
        {
            vkDestroySemaphore(device.GetVkDevice(), semaphore, nullptr);
        }

        vkDestroyFence(device.GetVkDevice(), inFlightFence, nullptr);
    }

    VkSemaphore& Frame::GetSemaphore(const uint32_t semaphoreID)
    {
        return semaphores[semaphoreID];
    }

    VkFence& Frame::GetInFlightFence()
    {
        return inFlightFence;
    }

    FrameManager::FrameManager(const uint32_t firstSemaphore, const uint32_t lastSemaphore, const uint32_t numSemaphoresPerFrame, const uint32_t numFrames, const Device* device) : device{device ? *device : GetDevice()}, framesCount{numFrames}
    {		
        for (size_t i = 0; i < numFrames; ++i)
        {
            Ptr<Frame> frame = CreatePtr<Frame>(firstSemaphore, lastSemaphore, numSemaphoresPerFrame);
            frames.push_back(std::move(frame));
        }

        imagesInFlight.resize(GetSwapChain().GetImageViews().size());
    }

    FrameManager::~FrameManager()
    {
        // for (size_t i = 0; i < frames.size(); ++i)
        // {
        //     delete frames[i];
        // }
    }

    uint32_t FrameManager::AcquireSwapChainImage()
    {
        Frame* frame = GetCurrentFrame();			
        uint32_t image_index = GetSwapChain().AcquireImage(frame->GetSemaphore(frame->firstSemaphore));

        if (imagesInFlight[image_index] != VK_NULL_HANDLE)
        {
            vkWaitForFences(device.GetVkDevice(), 1, &imagesInFlight[image_index], VK_TRUE, UINT64_MAX);
        }
        
        vkWaitForFences(device.GetVkDevice(), 1, &frame->GetInFlightFence(), VK_TRUE, UINT64_MAX);
        vkResetFences(device.GetVkDevice(), 1, &frame->GetInFlightFence());
        imagesInFlight[image_index] = frame->GetInFlightFence();

        return image_index;
    }

    void FrameManager::Present()
    {			
        Frame* frame = GetCurrentFrame();
        GetSwapChain().Present(&frame->GetSemaphore(frame->lastSemaphore), 1);
        NextFrame();
    }

    void FrameManager::NextFrame()
    {		
        currentFrame = (currentFrame + 1) % framesCount;
    }

    Frame* FrameManager::GetCurrentFrame()
    {
        return frames[currentFrame].get();
    }

    uint32_t FrameManager::GetAmountOfFrames() const
    {
        return framesCount;
    }

    uint32_t FrameManager::GetCurrentFrameIndex() const
    {
        return currentFrame;
    }
}