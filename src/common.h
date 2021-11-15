#ifndef __COMMON_H__
#define __COMMON_H__

#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <map>

#define EXCEPTION()\
    std::runtime_error(std::string(__FILE__) + "\n\nLine: " + std::to_string(__LINE__))

#define EXCEPTION_WHAT(x)\
    std::runtime_error(std::string(__FILE__) + "\n\nLine: " + std::to_string(__LINE__) + "\n\n" + x)

#define LINE_OUT(x)\
    std::cout << x << '\n'

#ifndef PROJECT_ROOT_DIR
#   define PROJECT_ROOT_DIR ""
#endif

#endif