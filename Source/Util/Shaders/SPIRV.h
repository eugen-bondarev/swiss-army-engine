#ifndef __Util_Shaders_SPIRV_h__
#define __Util_Shaders_SPIRV_h__

#pragma once

#include "../Assets.h"

namespace Util
{
    namespace SPIRV
    {
        void CompileToFile(const Util::Path& pathToGLSL, const Util::Path& pathToOutput);
        Util::TextAsset CompileAndExtract(const Util::Path& pathToGLSL);
    }
}

#endif