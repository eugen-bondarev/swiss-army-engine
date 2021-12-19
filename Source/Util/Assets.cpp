#include "assets.h"

#include <stdexcept>
#include <fstream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Util
{
    void RemoveFile(const Path& filePath)
    {
        if (remove(filePath().c_str()) != 0)
        {
            throw std::runtime_error("Failed to remove file " + filePath());
        }
    }

    TextAsset LoadTextFile(const Path& filePath, const bool binary)
    {
        std::ifstream file;
        
        if (binary)
        {
            file.open(filePath(), std::ios::binary);
        }
        else
        {
            file.open(filePath());
        }

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open text file " + filePath());
        }

        std::string cont = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        return cont;
    }

    ImageAsset LoadImageFile(const Path& filePath)
    {
        ImageAsset result;

        int _width, _height, _numChannels;
        result.data = stbi_load(filePath().c_str(), &_width, &_height, &_numChannels, STBI_rgb_alpha);

        if (!result.data)
        {
            throw std::runtime_error("Failed to open image file " + filePath());
        }

        result.size = { static_cast<unsigned int>(_width), static_cast<unsigned int>(_height) };
        result.numChannels = static_cast<unsigned int>(_numChannels);
        result.mipLevels = static_cast<unsigned int>(std::floor(std::log2(std::max(result.size.x, result.size.y)))) + 1;

        return result;
    }

    ImageAsset LoadImageFile(const srm::Resource& resource)
    {
        ImageAsset result;

        int _width, _height, _numChannels;
        //result.data = stbi_load(filePath().c_str(), &_width, &_height, &_numChannels, STBI_rgb_alpha);
        result.data = stbi_load_from_memory(
            reinterpret_cast<const unsigned char*>(resource.data.data()),
            resource.data.size(),
            &_width, &_height, &_numChannels, STBI_rgb_alpha
        );

        if (!result.data)
        {
            throw std::runtime_error("Failed to load image file.");
        }

        result.size = { static_cast<unsigned int>(_width), static_cast<unsigned int>(_height) };
        result.numChannels = static_cast<unsigned int>(_numChannels);
        result.mipLevels = static_cast<unsigned int>(std::floor(std::log2(std::max(result.size.x, result.size.y)))) + 1;

        return result;
    }

    ModelAsset<PredefinedVertexLayouts::Vertex3D> LoadModelFile(const Path& filePath)
    {
        ModelAsset<PredefinedVertexLayouts::Vertex3D> result;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene)
        {
            throw std::runtime_error("Failed to open model file " + filePath());
        }

        const size_t meshIndex{ 0 };

        // for (size_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const aiMesh* mesh{ scene->mMeshes[meshIndex] };
            result.vertices.resize(mesh->mNumVertices);

            for (size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
            {
                const aiVector3D& position = mesh->mVertices[vertexIndex];
                const aiVector2D& texCoords = { mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y };
                const aiVector3D& normal = mesh->mNormals[vertexIndex];

                memcpy(&result.vertices[vertexIndex].position, &position, sizeof(float) * 3);
                memcpy(&result.vertices[vertexIndex].texCoords, &texCoords, sizeof(float) * 2);
                memcpy(&result.vertices[vertexIndex].normal, &normal, sizeof(float) * 3);
            }

            result.indices.resize(mesh->mNumFaces * 3);

            const unsigned int indicesPerFace = 3;

            for (int i = 0; i < mesh->mNumFaces; ++i)
            {
                for (int j = 0; j < indicesPerFace; ++j)
                {
                    result.indices[i * indicesPerFace + j] = mesh->mFaces[i].mIndices[j];
                }
            }
        }

        return result;
    }

    ModelAsset<PredefinedVertexLayouts::Vertex3D> LoadModelFile(const srm::Resource& resource)
    {
        ModelAsset<PredefinedVertexLayouts::Vertex3D> result;

        Assimp::Importer importer;
        //const aiScene* scene = importer.ReadFile(filePath(), aiProcess_Triangulate | aiProcess_FlipUVs);
        const aiScene* scene = importer.ReadFileFromMemory(
            resource.data.data(),
            resource.data.size(),
            aiProcess_Triangulate | aiProcess_FlipUVs
        );

        if (!scene)
        {
            throw std::runtime_error("Failed to load model file.");
        }

        const size_t meshIndex{ 0 };

        // for (size_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const aiMesh* mesh{ scene->mMeshes[meshIndex] };
            result.vertices.resize(mesh->mNumVertices);

            for (size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
            {
                const aiVector3D& position = mesh->mVertices[vertexIndex];
                const aiVector2D& texCoords = { mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y };
                const aiVector3D& normal = mesh->mNormals[vertexIndex];

                memcpy(&result.vertices[vertexIndex].position, &position, sizeof(float) * 3);
                memcpy(&result.vertices[vertexIndex].texCoords, &texCoords, sizeof(float) * 2);
                memcpy(&result.vertices[vertexIndex].normal, &normal, sizeof(float) * 3);
            }

            result.indices.resize(mesh->mNumFaces * 3);

            const unsigned int indicesPerFace = 3;

            for (int i = 0; i < mesh->mNumFaces; ++i)
            {
                for (int j = 0; j < indicesPerFace; ++j)
                {
                    result.indices[i * indicesPerFace + j] = mesh->mFaces[i].mIndices[j];
                }
            }
        }

        return result;
    }
}