#ifndef __VK_Logic_Spene_Space_h__
#define __VK_Logic_Spene_Space_h__

#pragma once

#include "../../../Logic/ISpace.h"
#include "SceneUBO.h"

namespace VK
{
    class Space : public Logic::ISpace
    {
    public:
        Space(SceneUBO* ubo);

        void UpdateProjectionMatrix() override;

    private:
        SceneUBO* ubo;
    };    
}

#endif