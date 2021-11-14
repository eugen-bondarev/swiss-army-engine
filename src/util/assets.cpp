#include "assets.h"

#include <stdexcept>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Util
{
    TextAsset LoadTextFile(const std::string& FileName)
    {
        std::ifstream file(FileName);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file " + FileName);
        }

        std::string cont = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        return cont;
    }

    ImageAsset LoadImageFile(const std::string& FileName)
    {
        ImageAsset result;

        int _width, _height, _numChannels;
        result.Data = stbi_load(FileName.c_str(), &_width, &_height, &_numChannels, STBI_rgb_alpha);
        // result.Data = stbi_load(FileName.c_str(), &_width, &_height, &_numChannels, 0);

        if (!result.Data)
        {
            throw std::runtime_error("Failed to open file " + FileName);
        }

        result.Width = static_cast<unsigned int>(_width);
        result.Height = static_cast<unsigned int>(_height);
        result.NumChannels = static_cast<unsigned int>(_numChannels);

        return result;
    }
}