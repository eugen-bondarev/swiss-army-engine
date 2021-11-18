#ifndef __DX_COMMON_H__
#define __DX_COMMON_H__

#pragma once

#include "../Common.h"
#include <d3d11.h>
#include <wrl.h>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#ifndef NDEBUG
#   define D3D_TRY(exp)\
        ::DX::GetDebugger()->Start();\
        exp;\
        ::DX::GetDebugger()->End(__FILE__, __LINE__)
#else
#   define D3D_TRY(exp)\
        exp
#endif

#ifndef NDEBUG
#define __DX_TRY(hr)\
    if (FAILED(hr)) { throw EXCEPTION_WHAT(#hr); }(void(0))
#else
#define __DX_TRY(hr)\
    hr
#endif

#endif