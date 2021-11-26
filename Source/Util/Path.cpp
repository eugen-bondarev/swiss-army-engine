#include "Path.h"

namespace Util
{
    Path::Path(const std::string& pathRelativeToProjectRoot)
    {
        (*this) = pathRelativeToProjectRoot;
    }

    Path::Path(const char* pathRelativeToProjectRoot)
    {
        (*this) = pathRelativeToProjectRoot;
    }

    Path::Path(const Path& path)
    {
        (*this) = path;
    }

    Path& Path::operator=(const std::string& pathRelativeToProjectRoot)
    {
        fullPath = pathRelativeToProjectRoot;
        return (*this);
    }

    Path& Path::operator=(const char* pathRelativeToProjectRoot)
    {
        fullPath = pathRelativeToProjectRoot;
        return (*this);
    }

    std::string Path::Resolve() const
    {
        return projectRoot + "/" + fullPath;
    }
    
    std::string Path::operator()() const
    {
        return Resolve();
    }

    Path Path::operator+(const char* string) const
    {
        return fullPath + string;
    }
    
    Path& Path::operator+=(const char* string)
    {
        (*this) = fullPath + string;
        return (*this);
    }
}