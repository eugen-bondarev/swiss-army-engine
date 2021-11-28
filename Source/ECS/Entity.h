// #ifndef __ECS_Entity_h__
// #define __ECS_Entity_h__

// #pragma once

// #include "../Common/Common.h"
// #include "Component.h"
// #include "Transform.h"
// #include "Factory.h"

// #include <unordered_map>

// class Entity
// {
// public:
//     Entity();
    
//     template <typename T>
//     T* AddComponent()
//     {
//         Ptr<T> component {CreatePtr<T>()};
//         T* rawPtr {component.get()};
//         components[T::GetTypeName()] = std::move(component);
//         if (T::GetTypeName() == "Transform") { this->transform = dynamic_cast<Transform*>(rawPtr); }
//         return rawPtr;
//     }

//     Component* AddComponent(const std::string& name)
//     {
//         Component* rawPtr {reinterpret_cast<Component*>(ECSFactory::Instantiate(name))};
//         Ptr<Component> component {rawPtr};
//         components[name] = std::move(component);
//         if (rawPtr->GetComponentName() == "Transform") { this->transform = dynamic_cast<Transform*>(rawPtr); }
//         return rawPtr;
//     }

//     Component* GetComponent(const std::string& name)
//     {
//         return components[name].get();
//     }

//     template <typename T>
//     T* GetComponent()
//     {
//         return reinterpret_cast<T*>(components[T::GetTypeName()].get());
//     }

//     Transform* GetTransform()
//     {
//         return transform;
//     }

// private:
//     std::unordered_map<std::string, Ptr<Component>> components;
//     Transform* transform;
// };

// #endif