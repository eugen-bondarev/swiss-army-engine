#ifndef __DX_Common_h__
#define __DX_Common_h__

#pragma once

#include "../Common/Common.h"
#include <d3d11.h>
#include <wrl.h>

namespace DX
{
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
}

#ifndef NDEBUG
#   define DX_TRY(exp)\
        ::DX::GetDebugger()->Start();\
        exp;\
        ::DX::GetDebugger()->End(__FILE__, __LINE__)
#else
#   define DX_TRY(exp)\
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