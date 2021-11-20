#include "Instance.h"

#include "../DX/Instance.h"

#include <map>
    
namespace API
{
    static thread_local std::map<Window*, Instance*>     instances;
    static thread_local Instance*                        currentInstance{nullptr};

    Instance::Instance(Window& window) : window{window}
    {
        instances[&window] = this;
        currentInstance = this;
    }

    Ptr<Instance> Instance::Create(Window& window, const Type type)
    {
        if (type == Type::DirectX) return CreatePtr<DX::Instance>(window);
        return nullptr;
    }

    void MakeInstanceCurrent(Instance* newContext)
    {
        currentInstance = newContext;
    }

    Instance* GetInstance(Window* window)
    {
        return instances[window];
    }

    Instance* GetCurrentInstance()
    {
        return currentInstance;
    }
}