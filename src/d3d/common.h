#ifndef __D3D_COMMON_H__
#define __D3D_COMMON_H__

#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define EXCEPTION()\
    std::runtime_error("File: " + std::string(__FILE__) + "\nLine: " + std::to_string(__LINE__))

#define EXCEPTION_WHAT(x)\
    std::runtime_error(std::string(__FILE__) + "\n\nLine: " + std::to_string(__LINE__) + "\n\n" + x)

#ifndef NDEBUG
#define D3D_CHECK(hr)\
    if (FAILED(hr)) { throw EXCEPTION_WHAT(#hr); }(void(0))
#else
#define D3D_CHECK(hr)\
    hr
#endif

#endif