#include "assets.h"

#include <stdexcept>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Util
{
    TextAsset LoadTextFile(const std::string& FileName)
    {
        std::ifstream file(FileName);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open text file " + FileName);
        }

        std::string cont = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        return cont;
    }

    ImageAsset LoadImageFile(const std::string& FileName)
    {
        ImageAsset result;

        int _width, _height, _numChannels;
        result.Data = stbi_load(FileName.c_str(), &_width, &_height, &_numChannels, STBI_rgb_alpha);

        if (!result.Data)
        {
            throw std::runtime_error("Failed to open image file " + FileName);
        }

        result.Width = static_cast<unsigned int>(_width);
        result.Height = static_cast<unsigned int>(_height);
        result.NumChannels = static_cast<unsigned int>(_numChannels);

        return result;
    }

    ModelAsset LoadModelFile(const std::string& FileName)
    {
        ModelAsset result;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(FileName, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene)
        {
            throw std::runtime_error("Failed to open model file " + FileName);
        }

        // LINE_OUT(scene->mNumMeshes);

        const size_t meshIndex{0};

        // for (size_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const aiMesh* mesh{scene->mMeshes[meshIndex]};
	        result.Vertices.resize(mesh->mNumVertices);

            for (size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
            {
                const aiVector3D& position = mesh->mVertices[vertexIndex];
                const aiVector2D& texCoords = { mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y };
             // const aiVector3D& normal = mesh->mNormals[vertexIndex];

                memcpy(&result.Vertices[vertexIndex].position,  &position,  sizeof(float) * 3);
                memcpy(&result.Vertices[vertexIndex].texCoords, &texCoords, sizeof(float) * 2);
             // memcpy(&result.Vertices[vertexIndex].normal,    &normal,    sizeof(float) * 3);
            }

            result.Indices.resize(mesh->mNumFaces * 3);

            const unsigned int indicesPerFace = 3;

            for (int i = 0; i < mesh->mNumFaces; ++i)
            {
                for (int j = 0; j < indicesPerFace; ++j)
                {
                    result.Indices[i * indicesPerFace + j] = mesh->mFaces[i].mIndices[j];
                }
            }
        }

        return result;
    }
}