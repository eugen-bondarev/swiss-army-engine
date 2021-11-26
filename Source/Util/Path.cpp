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

    Path& Path::operator=(const std::string& pathRelativeToProjectRoot)
    {
        fullPath = projectRoot + "/" + pathRelativeToProjectRoot;
        return (*this);
    }

    Path& Path::operator=(const char* pathRelativeToProjectRoot)
    {
        fullPath = projectRoot + "/" + pathRelativeToProjectRoot;
        return (*this);
    }

    std::string Path::Resolve() const
    {
        return fullPath;
    }

    std::string Path::operator()() const
    {
        return Resolve();
    }
}