#ifndef __Util_Assets_h__
#define __Util_Assets_h__

#pragma once

#include "../Common/Common.h"
#include "../Common/Vertex.h"

namespace Util
{
    using TextAsset = std::string;

    struct ImageAsset
    {
        using Pixels = unsigned char*;

        unsigned int width{0};
        unsigned int height{0};
        unsigned int numChannels{0};
        Pixels       data{nullptr};
    };

    struct ModelAsset
    {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
    };

    TextAsset LoadTextFile(const std::string& filePath);
    ImageAsset LoadImageFile(const std::string& filePath);
    ModelAsset LoadModelFile(const std::string& filePath);
}

#endif