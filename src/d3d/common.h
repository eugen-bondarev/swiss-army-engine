#ifndef __D3D_COMMON_H__
#define __D3D_COMMON_H__

#pragma once

#include <d3d11.h>

namespace common
{
    extern ID3D11Device* device;
    extern ID3D11DeviceContext* ctx;
}

#endif