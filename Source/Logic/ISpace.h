#ifndef __Logic_ISpace_h__
#define __Logic_ISpace_h__

#pragma once

namespace Logic
{
    class ISpace
    {
    public:
        virtual void UpdateProjectionMatrix() = 0;

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
        float farVal
         {1000.0f};
    };
}

#endif