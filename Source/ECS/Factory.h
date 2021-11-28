#ifndef __ECS_Factory_h__
#define __ECS_Factory_h__

#pragma once

#include <string>
#include <memory>
#include <map>

namespace ECSFactory
{
    struct DeclaratorBase
    {
        virtual void* Instantiate() const = 0;
    };

    template <typename T>
    struct Declarator : public DeclaratorBase
    {
        void* Instantiate() const override
        {
            return new T();
        }
    };

    struct Factory
    {
        std::map<std::string, std::shared_ptr<DeclaratorBase>> bases;
        std::map<std::string, size_t> sizesInBytes;
    } inline factory;

    void* Instantiate(const std::string& name);
    size_t GetSizeOf(const std::string& name);

    template <typename T>
    T* Instantiate(const std::string& name)
    {
        return reinterpret_cast<T*>(Instantiate(name));
    }
}

#define REGISTER_NAME(x)\
    static std::string GetTypeName()\
    {\
        return #x;\
    }\
    std::string GetComponentName() const override\
    {\
        return #x;\
    }

#define DECLARE_COMPONENT(x)\
    struct Declarator##x\
    {\
        Declarator##x()\
        {\
            ::ECSFactory::factory.bases[#x] = std::make_shared<::ECSFactory::Declarator<x>>();\
            ::ECSFactory::factory.sizesInBytes[#x] = sizeof(x);\
        }\
    } inline declarator##x

#endif