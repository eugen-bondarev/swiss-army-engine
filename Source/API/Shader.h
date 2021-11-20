#ifndef __API_Shader_h__
#define __API_Shader_h__

#pragma once

#include "Instance.h"
#include "../Common/Common.h"

namespace API
{
    class Shader
    {
    public:
        static Ptr<Shader> Create(const std::string& vsCode, const std::string& fsCode);
        virtual void Bind() = 0;
    };
}

#endif