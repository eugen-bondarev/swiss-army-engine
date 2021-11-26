#ifndef __Util_Shaders_SPIRV_h__
#define __Util_Shaders_SPIRV_h__

#pragma once

#include "../Assets.h"

namespace Util
{
    namespace SPIRV
    {
        void CompileToFile(const Util::Path& pathToGLSL, const Util::Path& pathToOutput);

        template <typename... Args>
        Util::TextAsset CompileAndExtract(const Util::Path& pathToGLSL, Args&&... args)
        {
            const Util::Path pathToOutput {pathToGLSL + ".out.temp"};
            CompileToFile(pathToGLSL, pathToOutput);
            const Util::TextAsset result {Util::LoadTextFile(pathToOutput, std::forward<Args>(args)...)};
            Util::RemoveFile(pathToOutput);
            return result;
        }
    }
}

#endif