#ifndef __VK_Logic_Spene_PerspectiveSpace_h__
#define __VK_Logic_Spene_PerspectiveSpace_h__

#pragma once

#include "../../../Logic/IPerspective.h"
#include "SceneUBO.h"

namespace VK
{
    class PerspectiveSpace : public Logic::IPerspective
    {
    public:
        PerspectiveSpace(SceneUBO* ubo);

        void UpdateProjectionMatrix() override;

        Vec3f rightVector {0.0f};
        Vec3f forwardVector {0.0f};

        struct
        {
            Vec3f position {0.0f};
            Vec3f rotation {0.0f};
        } camera;

    private:
        SceneUBO* ubo;
    };    
}

#endif