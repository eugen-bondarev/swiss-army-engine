#ifndef __Logic_ISpaceLogic_h__
#define __Logic_ISpaceLogic_h__

#pragma once

#include "../Common/Common.h"

namespace Logic
{
    class ISpaceObject
    {
    public:
        virtual void UpdateModelMatrix() = 0;

        template <typename... Args>
        void SetPosition(Args&&... args) { position = Vec3f {std::forward<Args>(args)...}; UpdateModelMatrix(); }
        void SetPositionX(const float value) { position.x = value; UpdateModelMatrix(); }
        void SetPositionY(const float value) { position.y = value; UpdateModelMatrix(); }
        void SetPositionZ(const float value) { position.z = value; UpdateModelMatrix(); }

        template <typename... Args>
        void SetRotation(Args&&... args) { rotation = Vec3f {std::forward<Args>(args)...}; UpdateModelMatrix(); }
        void SetRotationX(const float value) { rotation.x = value; UpdateModelMatrix(); }
        void SetRotationY(const float value) { rotation.y = value; UpdateModelMatrix(); }
        void SetRotationZ(const float value) { rotation.z = value; UpdateModelMatrix(); }

    protected:
        Vec3f position {0.0f};
        Vec3f rotation {0.0f};
    };    
}

#endif