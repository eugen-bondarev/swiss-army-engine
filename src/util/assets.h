#ifndef __UTIL_ASSETS_H__
#define __UTIL_ASSETS_H__

#pragma once

#include <string>

namespace Util
{
    using TextAsset = std::string;

    struct ImageAsset
    {
        using Pixels_t = unsigned char*;

        unsigned int Width{0};
        unsigned int Height{0};
        unsigned int NumChannels{0};
        Pixels_t Data{nullptr};
    };

    TextAsset LoadTextFile(const std::string& FileName);
    ImageAsset LoadImageFile(const std::string& FileName);
}

#endif