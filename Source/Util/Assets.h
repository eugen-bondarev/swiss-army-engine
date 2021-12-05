#ifndef __Util_Assets_h__
#define __Util_Assets_h__

#pragma once

#include "../Common/Common.h"
#include "../Common/Vertex.h"
#include "Path.h"

namespace Util
{
    using TextAsset = std::string;

    struct ImageAsset
    {
        using Pixels = unsigned char*;

        Vec2ui       size;
        unsigned int numChannels{0};
        Pixels       data{nullptr};
        unsigned int mipLevels{1};
    };

    struct ModelAsset
    {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
    };

    void RemoveFile(const Path& filePath);
    TextAsset LoadTextFile(const Path& filePath, const bool binary = false);
    ImageAsset LoadImageFile(const Path& filePath);
    ModelAsset LoadModelFile(const Path& filePath);
}

#endif