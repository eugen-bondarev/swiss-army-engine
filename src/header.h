#ifndef __HEADER_H__
#define __HEADER_H__

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <vector>
#include <array>
#include <map>

#include <stdexcept>
#include <iostream>

#define LINE_OUT(x) std::cout << x << '\n'

#define CHECK_RESULT(result, msg)\
	if (FAILED(result))\
	{\
        throw std::runtime_error(msg);\
	}

#endif