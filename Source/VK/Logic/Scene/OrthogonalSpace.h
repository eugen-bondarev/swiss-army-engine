#ifndef __VK_Logic_Spene_Space_h__
#define __VK_Logic_Spene_Space_h__

#pragma once

#include "../../../Logic/IOrthogonal.h"
#include "SceneUBO.h"

namespace VK
{
    class OrthogonalSpace : public Logic::IOrthogonal
    {
    public:
        OrthogonalSpace(SceneUBO* ubo);

        void UpdateProjectionMatrix() override;

    private:
        SceneUBO* ubo;
    };    
}

#endif