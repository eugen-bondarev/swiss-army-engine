#include "SPIRV.h"

namespace Util
{
    namespace SPIRV
    {
        void CompileToFile(const Util::Path& pathToGLSL, const Util::Path& pathToOutput)
        {
            const std::string compileShaderCommand 
            {
                "glslc "        +
                pathToGLSL()    +
                " -o "          +
                pathToOutput()
            };
            const int result {system(compileShaderCommand.c_str())};
        }
        
        Util::TextAsset CompileAndExtract(const Util::Path& pathToGLSL)
        {
            const Util::Path pathToOutput {pathToGLSL + ".out.temp"};
            CompileToFile(pathToGLSL, pathToOutput);
            const Util::TextAsset result {Util::LoadTextFile(pathToOutput, true)};
            Util::RemoveFile(pathToOutput);
            return result;
        }
    }
}