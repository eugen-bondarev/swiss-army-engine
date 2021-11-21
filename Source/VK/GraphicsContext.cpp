#include "GraphicsContext.h"
#include "Entities/Vertex.h"

#include "../Util/Assets.h"

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

		// Creating descriptor set layout (for the pipeline)
		std::vector<VkDescriptorSetLayoutBinding> bindings = 
		{
			VK::CreateBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
		};

		descriptorSetLayout = CreatePtr<VK::DescriptorSetLayout>(bindings);

		const VK::AttachmentDescriptions attachments = { VK::Util::CreateAttachment(VK_FORMAT_B8G8R8A8_UNORM) };
		const VK::BindingDescriptions bindingDescriptors = VK::Vertex::GetBindingDescriptions();
		const VK::AttributeDescriptions attributeDescriptors = VK::Vertex::GetAttributeDescriptions();
		const std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { descriptorSetLayout->GetVkDescriptorSetLayout() };

        ::Util::TextAsset vsCode = ::Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.vert.spv", true);
        ::Util::TextAsset fsCode = ::Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/FragmentShader.frag.spv", true);

        pipeline = CreatePtr<Pipeline>(
            vsCode, fsCode,
			800, 600,
			attachments,
			bindingDescriptors, 
			attributeDescriptors,
			descriptorSetLayouts
        );

		std::vector<VkWriteDescriptorSet> writeDescs = 
		{
			// VK::CreateWriteDescriptorSet(descriptorSet.get(), 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &block.tileMap->GetImageView()->GetDescriptor())
		};

        descriptorPool = CreatePtr<DescriptorPool>();
        descriptorSet = CreatePtr<DescriptorSet>(descriptorPool.get(), descriptorSetLayouts);
        
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

    SwapChain* GetSwapChain()
    {
        return dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->swapChain.get();
    }

    CommandBuffer* GetCommandBuffer()
    {
        return dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->commandBuffer.get();
    }

    CommandPool* GetCommandPool()
    {
        return dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->commandPool.get();
    }
}