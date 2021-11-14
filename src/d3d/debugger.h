#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#pragma once

#include "../header.h"

#include <dxgidebug.h>

class Debugger
{
public:
    Debugger();

    size_t next{0u};
    size_t end{0u};

    void Start();
    void End(const std::string& file, const unsigned int line);

	std::vector<std::string> messages;

    ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
};

#endif