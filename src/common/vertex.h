#ifndef __VERTEX_H__
#define __VERTEX_H__

#pragma once

#include "../common.h"

struct Vertex
{
    struct
    {
        float x, y, z;
    } position;

    struct
    {
        float x, y;
    } texCoords;
};

#endif