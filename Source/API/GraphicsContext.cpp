#include "GraphicsContext.h"

#include "../DX/GraphicsContext.h"
#include "../VK/GraphicsContext.h"

#include <map>
    
namespace API
{
    static thread_local std::map<RawWindow*, GraphicsContext*> graphicsContexts;
    static thread_local GraphicsContext* currentGraphicsContext{nullptr};

    GraphicsContext::GraphicsContext(RawWindow& window) : window{window}
    {
        graphicsContexts[&window] = this;
        currentGraphicsContext = this;
    }

    Ptr<GraphicsContext> GraphicsContext::Create(RawWindow& window, const Type type)
    {
        if (type == Type::DirectX) return CreatePtr<DX::GraphicsContext>(window);
        if (type == Type::Vulkan) return CreatePtr<VK::GraphicsContext>(window);
        return nullptr;
    }

    void MakeGraphicsContextCurrent(GraphicsContext* newContext)
    {
        currentGraphicsContext = newContext;
    }

    GraphicsContext* GetGraphicsContext(RawWindow* window)
    {
        return graphicsContexts[window];
    }

    GraphicsContext* GetCurrentGraphicsContext()
    {
        return currentGraphicsContext;
    }

    RawWindow& GraphicsContext::GetWindow()
    {
        return window;
    }
}