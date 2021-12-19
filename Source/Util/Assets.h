#ifndef __Util_Assets_h__
#define __Util_Assets_h__

#pragma once

#include "../Common/Common.h"
#include "../Common/Vertex.h"
#include "Path.h"

#include <SRM/Resource.h>

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

    template <typename T>
    struct ModelAsset
    {
        std::vector<T>       vertices;
        std::vector<unsigned int> indices;
    };

    void RemoveFile(const Path& filePath);

    TextAsset LoadTextFile(const Path& filePath, const bool binary = false);

    ImageAsset LoadImageFile(const Path& filePath);
    ImageAsset LoadImageFile(const srm::Resource& resource);

    ModelAsset<PredefinedVertexLayouts::Vertex3D> LoadModelFile(const Path& filePath);
    ModelAsset<PredefinedVertexLayouts::Vertex3D> LoadModelFile(const srm::Resource& resource);
}

#endif