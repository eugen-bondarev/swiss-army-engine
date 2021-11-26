#include "SPIRV.h"

namespace Util
{
    namespace SPIRV
    {
        void CompileToFile(const Util::Path& pathToGLSL, const Util::Path& pathToOutput)
        {
            const std::string compileVertexShaderCommand 
            {
                "glslc "        +
                pathToGLSL()    +
                " -o "          +
                pathToOutput()
            };
            const int result {system(compileVertexShaderCommand.c_str())};
        }
    }
}