#ifndef __HEADER_H__
#define __HEADER_H__

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>

#include <stdexcept>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <map>

#include <wrl.h>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define LINE_OUT(x) std::cout << x << '\n'

#define D3D_RELEASE(x)\
    if (x)\
    {\
        x->Release();\
        x = nullptr;\
    }(void(0))

#define CHECK_RESULT(result, msg)\
	if (FAILED(result))\
	{\
        throw std::runtime_error(msg);\
	}

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

template <typename T>
T create_structure()
{
    T structure;
    ZeroMemory(&structure, sizeof(structure));
    return structure;
}

#endif