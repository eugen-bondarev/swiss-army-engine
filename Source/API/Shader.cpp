#include "Shader.h"

#include "../DX/Shader.h"

namespace API
{    
    Ptr<Shader> Shader::Create(const std::string& vsCode, const std::string& fsCode)
    {
        if (GetCurrentGraphicsContext()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::Shader>(vsCode, fsCode);
        return nullptr;
    }
}