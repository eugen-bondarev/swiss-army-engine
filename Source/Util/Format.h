#ifndef __Util_Format_h__
#define __Util_Format_h__

#pragma once

#include <string>

namespace Util
{
    template<typename... Args>
    std::string Format(const std::string& format, Args... args)
    {
        const int size_s{std::snprintf(nullptr, 0, format.c_str(), args...) + 1};

        if (size_s <= 0) 
        { 
            throw std::runtime_error( "Error during formatting." ); 
        }

        const size_t size{static_cast<size_t>(size_s)};
        const std::unique_ptr<char[]> buf{std::make_unique<char[]>(size)};
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1);
    }
}

#endif