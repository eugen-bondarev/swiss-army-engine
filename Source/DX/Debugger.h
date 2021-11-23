#ifndef __DX_Debugger_h__
#define __DX_Debugger_h__

#pragma once

#include "Common.h"

#include <dxgidebug.h>

namespace DX
{
    class Debugger
    {
    public:
        Debugger();

        void Start();
        void End(const std::string& file, const unsigned int line);

    private:
        size_t next{0u};
        std::vector<std::string> messages;
        ComPtr<IDXGIInfoQueue> dxInfoQueue;
    };
}

#endif