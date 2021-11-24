#ifndef __VK_Frame_Frame_h__
#define __VK_Frame_Frame_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
    class Frame
    {
    friend class FrameManager;

    public:
        Frame(const uint32_t firstSemaphore, const uint32_t lastSemaphore, const uint32_t numSemaphores, const Device* device = nullptr);
       ~Frame();

        const VkSemaphore& GetSemaphore(const uint32_t semaphoreID) const;
        const VkFence& GetInFlightFence() const;

        std::vector<VkSemaphore> semaphores;

    private:
        const Device& device;
        uint32_t firstSemaphore;
        uint32_t lastSemaphore;

        VkFence inFlightFence;

        Frame(const Frame&) = delete;
        Frame& operator=(const Frame&) = delete;
    };

    class FrameManager
    {
    public:
        FrameManager(const uint32_t firstSemaphore, const uint32_t lastSemaphore, const uint32_t numSemaphoresPerFrame, const uint32_t numFrames, const Device* device = nullptr);
       ~FrameManager() = default;

        const Frame* GetCurrentFrame() const;
        uint32_t GetAmountOfFrames() const;
        uint32_t GetCurrentFrameIndex() const;
        uint32_t AcquireSwapChainImage();

        void NextFrame();
        void Present();
        
    private:
        const Device& device;
        uint32_t framesCount;
        uint32_t currentFrame{0};
        std::vector<Ptr<Frame>> frames;
        std::vector<VkFence> imagesInFlight;

        FrameManager(const FrameManager&) = delete;
        FrameManager& operator=(const FrameManager&) = delete;
    };
}

#endif