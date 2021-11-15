#ifndef __D3D_D3D11_DEBUGGER_H__
#define __D3D_D3D11_DEBUGGER_H__

#pragma once

#include "common.h"

#include <dxgidebug.h>

class ID3D11Debugger
{
public:
    ID3D11Debugger();

    void Start();
    void End(const std::string& file, const unsigned int line);

private:
    size_t                      Next{0u};
	std::vector<std::string>    Messages;
    ComPtr<IDXGIInfoQueue>      DXInfoQueue;    
};

#endif