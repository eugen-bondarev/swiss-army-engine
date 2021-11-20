#include "Instance.h"

#include "../DX/Instance.h"

#include <map>
    
namespace API
{
    static thread_local std::map<RawWindow*, Instance*>     instances;
    static thread_local Instance*                        currentInstance{nullptr};

    Instance::Instance(RawWindow& window) : window{window}
    {
        instances[&window] = this;
        currentInstance = this;
    }

    Ptr<Instance> Instance::Create(RawWindow& window, const Type type)
    {
        if (type == Type::DirectX) return CreatePtr<DX::Instance>(window);
        return nullptr;
    }

    void MakeInstanceCurrent(Instance* newContext)
    {
        currentInstance = newContext;
    }

    Instance* GetInstance(RawWindow* window)
    {
        return instances[window];
    }

    Instance* GetCurrentInstance()
    {
        return currentInstance;
    }
}