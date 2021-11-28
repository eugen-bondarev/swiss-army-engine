#ifndef __VK_Logic_Scene_SpaceObject_h__
#define __VK_Logic_Scene_SpaceObject_h__

#pragma once

#include "../../../Logic/ISpaceObject.h"
#include "EntityUBO.h"

namespace VK
{
    class SpaceObject : public Logic::ISpaceObject
    {
    public:
        SpaceObject(EntityUBO* ubo);

        void UpdateModelMatrix() override;

    private:
        EntityUBO* ubo;
    };
}

#endif