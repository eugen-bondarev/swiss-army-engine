#ifndef __Common_Vertex_h__
#define __Common_Vertex_h__

#pragma once

#include "Common.h"

namespace PredefinedVertexLayouts
{
    struct Vertex3D
    {
        struct
        {
            float x, y, z;
        } position;

        struct
        {
            float x, y;
        } texCoords;

        struct
        {
            float x, y, z;
        } normal;
    };

    struct Vertex2D
    {
        struct
        {
            float x, y;
        } position;

        struct
        {
            float x, y;
        } texCoords;
    };
}

#endif