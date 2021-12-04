#ifndef __VK_Logic_Spene_Space_h__
#define __VK_Logic_Spene_Space_h__

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

    private:
        SceneUBO* ubo;
    };    
}

#endif