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

        VkSemaphore& GetSemaphore(const uint32_t semaphoreID);

        VkFence& GetInFlightFence();

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
       ~FrameManager();

        void NextFrame();
        Frame* GetCurrentFrame();

        uint32_t GetAmountOfFrames() const;
        uint32_t GetCurrentFrameIndex() const;

        uint32_t AcquireSwapChainImage();
        void Present();

        std::vector<VkFence> imagesInFlight;
        
    private:
        const Device& device;
        uint32_t framesCount;
        uint32_t currentFrame{0};
        std::vector<Ptr<Frame>> frames;

        FrameManager(const FrameManager&) = delete;
        FrameManager& operator=(const FrameManager&) = delete;
    };
}

#endif