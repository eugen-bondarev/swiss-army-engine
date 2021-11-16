#ifndef __DX_D3D11_DEBUGGER_H__
#define __DX_D3D11_DEBUGGER_H__

#pragma once

#include "Common.h"

#include <dxgidebug.h>

namespace DX {

class Debugger
{
public:
    Debugger();

    void Start();
    void End(const std::string& File, const unsigned int Line);

private:
    size_t                      Next{0u};
	std::vector<std::string>    Messages;
    ComPtr<IDXGIInfoQueue>      DXInfoQueue;    
};

}

#endif