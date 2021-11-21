#include "GraphicsContext.h"
#include "Entities/Vertex.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>();
        surface = CreatePtr<Surface>(*instance, window);
        device = CreatePtr<Device>(*instance, *surface);
        swapChain = CreatePtr<SwapChain>(window, *surface, *device);
        commandPool = CreatePtr<CommandPool>();
        commandBuffer = CreatePtr<CommandBuffer>(*commandPool);

		const VK::AttachmentDescriptions attachments = { VK::Util::CreateAttachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) };
		const VK::BindingDescriptions binding_descriptors = VK::Vertex::GetBindingDescriptions();
		const VK::AttributeDescriptions attribute_descriptors = VK::Vertex::GetAttributeDescriptions();
		// const std::vector<VkDescriptorSetLayout> descriptor_set_layouts = { descriptorSetLayout->GetVkDescriptorSetLayout() };

        // pipeline = CreatePtr<Pipeline>(
        //  "", ""
		// 	vs_code.GetContent(), fs_code.GetContent(), 
		// 	800, 600,
		// 	attachments,
		// 	binding_descriptors, 
		// 	attribute_descriptors,
		// 	descriptor_set_layouts
        // );
        
        window.SetSwapChain(swapChain.get());
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }

    Device* GetDevice()
    {
        return dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->device.get();
    }
}