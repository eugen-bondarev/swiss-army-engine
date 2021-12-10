#include "RendererImGui.h"

#include "../Descriptors/DescriptorPool.h"
#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include "../Instance/Instance.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"
#include <imgui_impl_glfw.h>

void ImGuiInit(
    GLFWwindow* handle, 
    const VkInstance& vkInstance, 
    const VkPhysicalDevice& vkPhysicalDevice,
    const VkDevice& vkDevice,
    const uint32_t queueFamily,
    const VkQueue& queue,
    const VK::DescriptorPool& descriptorPool,
    const VK::RenderPass& renderPass
)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(handle, true);
    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.Instance = vkInstance;
    initInfo.PhysicalDevice = vkPhysicalDevice;
    initInfo.Device = vkDevice;
    initInfo.QueueFamily = queueFamily;
    initInfo.Queue = queue;
    initInfo.PipelineCache = nullptr;
    initInfo.DescriptorPool = descriptorPool.GetVkDescriptorPool();
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo, renderPass.GetVkRenderPass());

    const VK::CommandPool pool;
    const VK::CommandBuffer cmd(pool);

    cmd.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        ImGui_ImplVulkan_CreateFontsTexture(cmd.GetVkCommandBuffer());
    cmd.End();

    cmd.SubmitToQueue(queue);

    vkDeviceWaitIdle(vkDevice);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGuiShutdown()
{    
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

namespace VK
{
    RendererImGui::RendererImGui(
        const size_t numCmdBuffers,
        const size_t samples,
        const RendererFlags flags,
        GraphicsContext& ctx
    ) : Renderer(numCmdBuffers, samples, flags, ctx)
    {
        needsResize.resize(GetNumCmdBuffers());

        for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
        newSize = ctx.GetWindow().GetSize();

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newViewportSize)
        {
            for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
            newSize = newViewportSize;
        });

        CreateGraphicsResources(
            samples,
            flags
        );

        ctx.GetWindow().EndFrameSubscribe([&]()
        {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        });

        ImGuiInit(
            ctx.GetWindow().GetHandle(),
            GetInstance().GetVkInstance(),
            GetDevice().GetVkPhysicalDevice(),
            GetDevice().GetVkDevice(),
            VK::Queues::indices.graphicsFamily.value(),
            VK::Queues::graphicsQueue,
            VK::GetDefaultDescriptorPool(),
            GetRenderPass()
        );
    }

    RendererImGui::~RendererImGui()
    {
        ImGuiShutdown();
    }

    void RendererImGui::CreateGraphicsResources(const size_t samples, const RendererFlags flags)
    {
        AttachmentDescriptions attachments;
        VkAttachmentDescription swapChainAttachment = GetSwapChain().GetDefaultAttachmentDescription(SamplesToVKFlags(samples));

        swapChainAttachment.finalLayout = 
            flags & RendererFlags_Output ?
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR :
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        swapChainAttachment.initialLayout =
            flags & RendererFlags_Load ?
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL :
                VK_IMAGE_LAYOUT_UNDEFINED;

        swapChainAttachment.loadOp =
            flags & RendererFlags_Load ?
                VK_ATTACHMENT_LOAD_OP_LOAD :
                VK_ATTACHMENT_LOAD_OP_DONT_CARE;

        attachments.push_back(swapChainAttachment);

        renderPass = CreatePtr<RenderPass>(
            attachments,
            samples,
            false,
            ctx.GetDevice()
        );

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, samples, false);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            // orthogonalSpace->SetAspectRatio(newSize.x / newSize.y);
            vkQueueWaitIdle(Queues::graphicsQueue);
            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, this->samples, false);
        });
    }

    void RendererImGui::Record(const size_t cmdIndex)
    {
        VK::CommandPool& pool = GetCommandPool(cmdIndex);
        VK::CommandBuffer& cmd = GetCommandBuffer(cmdIndex);
        const Framebuffer& framebuffer = renderTarget->GetFramebuffer(cmdIndex);

        pool.Reset();
        cmd.Begin();

            if (needsResize[cmdIndex])
            {
                cmd.SetViewport(newSize.x, newSize.y);
                cmd.SetScissors(newSize.x, newSize.y);
                needsResize[cmdIndex] = false;
            }

            cmd.BeginRenderPass(*renderPass, framebuffer);
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.GetVkCommandBuffer());
            cmd.EndRenderPass();
        cmd.End(); 
    }

    RenderPass& RendererImGui::GetRenderPass()
    {
        return *renderPass;
    }
}