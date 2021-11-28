// #ifndef __ECS_Transform_h__
// #define __ECS_Transform_h__

// #pragma once

// #include <gtc/matrix_transform.hpp>
// #include "../Common/Common.h"
// #include "Component.h"
// #include "Factory.h"

// class Transform : public Component
// {
// public:
//     REGISTER_NAME(Transform)

//     Transform();

//     void Init(PerEntityUBO* ubo)
//     {
//         this->ubo = ubo;
//     }

//     void UpdateMatrix()
//     {
//         ubo->model = glm::translate(glm::mat4x4(1), position);
//         ubo->model = glm::rotate(ubo->model, rotation.x, glm::vec3(1, 0, 0));
//         ubo->model = glm::rotate(ubo->model, rotation.y, glm::vec3(0, 1, 0));
//         ubo->model = glm::rotate(ubo->model, rotation.z, glm::vec3(0, 0, 1));
//     }

//     template <typename... Args>
//     void SetPosition(Args&&... args) { position = Vec3f {std::forward<Args>(args)...}; UpdateMatrix(); }
//     void SetPositionX(const float value);
//     void SetPositionY(const float value);
//     void SetPositionZ(const float value);

//     template <typename... Args>
//     void SetRotation(Args&&... args) { rotation = Vec3f {std::forward<Args>(args)...}; UpdateMatrix(); }
//     void SetRotationX(const float value);
//     void SetRotationY(const float value);
//     void SetRotationZ(const float value);

//     Vec3f GetPosition() const;
//     Vec3f GetRotation() const;

// protected:
//     PerEntityUBO* ubo;
//     Vec3f position {0};
//     Vec3f rotation {0};

//     Transform* parent {nullptr};
// };

// DECLARE_COMPONENT(Transform);

// #endif