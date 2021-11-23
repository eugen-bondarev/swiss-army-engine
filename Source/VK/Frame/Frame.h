#pragma once

#include "../Common.h"


namespace VK
{
    FORWARD_DECLARE(Device);
    
    class Frame
    {
    friend class FrameManager;

    public:
        Frame(int first_semaphore, int last_semaphore, int amount_of_semaphores, const Device* device = nullptr);
        ~Frame();

        VkSemaphore& GetSemaphore(int semaphore_id);

        VkFence& GetInFlightFence();

        std::vector<VkSemaphore> semaphores;

    private:
        const Device& device;
        int firstSemaphore;
        int lastSemaphore;

        VkFence inFlightFence;

        Frame(const Frame&) = delete;
        Frame& operator=(const Frame&) = delete;
    };

    class FrameManager
    {
    public:
        FrameManager(int first_semaphore, int last_semaphore, int amount_of_semaphores_per_frame, int frames_count, const Device* device = nullptr);
        ~FrameManager();

        void NextFrame();
        Frame* GetCurrentFrame();

        int GetAmountOfFrames() const;
        int GetCurrentFrameIndex() const;

        uint32_t AcquireSwapChainImage();
        void Present();

        std::vector<VkFence> imagesInFlight;
        
    private:
        const Device& device;
        int framesCount = 0;
        int currentFrame = 0;
        std::vector<Frame*> frames;

        FrameManager(const FrameManager&) = delete;
        FrameManager& operator=(const FrameManager&) = delete;
    };
}