#ifndef __Common_h__
#define __Common_h__

#pragma once

#include <stdexcept>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <map>

#include <typeinfo>  //for 'typeid' to work  

#define EXCEPTION()\
    std::runtime_error(std::string(__FILE__) + "\n\nLine: " + std::to_string(__LINE__))

#define EXCEPTION_WHAT(x)\
    std::runtime_error(std::string(__FILE__) + "\n\nLine: " + std::to_string(__LINE__) + "\n\n" + x)

#define LINE_OUT(x)\
    std::cout << x << '\n'

#ifndef PROJECT_ROOT_DIR
#   define PROJECT_ROOT_DIR ""
#endif

template <typename T>
using Ptr = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
auto CreatePtr(Args&&... args) -> decltype(std::make_unique<T>(std::forward<Args&&>(args)...))
{
    return std::make_unique<T>(std::forward<Args&&>(args)...);
}

template <typename T, typename... Args>
auto CreateRef(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args&&>(args)...))
{
    return std::make_shared<T>(std::forward<Args&&>(args)...);
}

#define FORWARD_DECLARE(x) class x

#define MY_ASSERT(x) assert(x)

#endif