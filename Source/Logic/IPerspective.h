#ifndef __Logic_IPerspective_h__
#define __Logic_IPerspective_h__

#pragma once

#include "ISpace.h"

namespace Logic
{
    class IPerspective : public ISpace
    {
    public:
        void SetFieldOfView(const float value) { fieldOfView = value; UpdateProjectionMatrix(); }
        void SetAspectRatio(const float value) { aspectRatio = value; UpdateProjectionMatrix(); }
        void SetNear(const float value) { nearVal = value; UpdateProjectionMatrix(); }
        void SetFar(const float value) { farVal = value; UpdateProjectionMatrix(); }
        void SetNearFar(const float nearValue, const float farValue) { nearVal = nearValue; farVal = farValue; UpdateProjectionMatrix(); }

        float GetFieldOfView() const { return fieldOfView; } 
        float GetAspectRatio() const { return aspectRatio; }
        float GetNear() const { return nearVal; } 
        float GetFar() const { return farVal; } 

    protected:        
        float fieldOfView {70.0f};
        float aspectRatio {1.0f};
        float nearVal {0.1f};
        float farVal {1000.0f};        
    };
}

#endif