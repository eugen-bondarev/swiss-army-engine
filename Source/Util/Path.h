#ifndef __Util_Path_h__
#define __Util_Path_h__

#include <string>

namespace Util
{
    class Path
    {
    public:
        inline static std::string projectRoot = PROJECT_ROOT_DIR;
        
        Path(const char* pathRelativeToProjectRoot);
        Path(const std::string& pathRelativeToProjectRoot);
        Path& operator=(const std::string& pathRelativeToProjectRoot);
        Path& operator=(const char* pathRelativeToProjectRoot);
        std::string Resolve() const;
        std::string operator()() const;        

    private:
        std::string fullPath;
    };
}

#endif