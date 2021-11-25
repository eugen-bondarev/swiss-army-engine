// #include "Window/RawWindow.h"
// #include "Common/Vertex.h"
// #include "Util/Assets.h"
// #include "API/Sampler.h"
// #include "API/Window.h"
// #include "API/Buffer.h"
// #include "API/Shader.h"
// #include "DX/DX.h"

// static Ptr<API::VertexBuffer>           meshVertexBuffer{nullptr};
// static Ptr<API::IndexBuffer>            meshIndexBuffer{nullptr};
// static Ptr<DX::MappableConstantBuffer>  constantBuffer{nullptr};
// static Ptr<API::Shader>                 shader{nullptr};
// static Ptr<API::Sampler>                sampler{nullptr};
// static Ptr<API::Texture>                texture{nullptr};

// void RenderMesh(const float angleX, const float angleY, const unsigned int numIndices)
// {
//     meshVertexBuffer->Bind(0u);
//     meshIndexBuffer->Bind();
//     constantBuffer->Bind();
//     shader->Bind();
//     sampler->Bind();
//     texture->Bind();

//     const DX::XMMATRIX transform = 
//         DX::XMMatrixTranspose(
//             DX::XMMatrixScaling(1, 1, 1) *
//             DX::XMMatrixRotationX(angleX) * 
//             DX::XMMatrixRotationY(angleY + M_PI) * 
//             DX::XMMatrixRotationZ(0) * 
//             DX::XMMatrixTranslation(0, -5, 10) * 
//             DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, DX::GetSwapChain()->GetAspectRatio(), 0.1f, 1000.0f)
//         );

//     DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer->Map());
//         *data = transform;
//     constantBuffer->Unmap();

//     // DX_TRY(DX::GetDevice().GetDxContext().UpdateSubresource(constantBuffer->GetDxBuffer(), 0, nullptr, &transform, 0, 0));

//     DX::GetDevice().GetDxContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

//     DX_TRY(DX::GetDevice().GetDxContext().DrawIndexed(numIndices, 0u, 0u));
// }

// void InitResources(const Util::TextAsset& vsCode, const Util::TextAsset& psCode, const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture)
// {
//     meshVertexBuffer = API::VertexBuffer::Create(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
//     meshIndexBuffer = API::IndexBuffer::Create(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
//     // constantBuffer = API::UniformBuffer::Create(sizeof(DX::XMMATRIX), 0, nullptr);
//     constantBuffer = CreatePtr<DX::MappableConstantBuffer>(sizeof(DX::XMMATRIX), 0, nullptr);
//     shader = API::Shader::Create(vsCode, psCode);
//     sampler = API::Sampler::Create();
//     texture = API::Texture::Create(characterTexture.size, characterTexture.data);
// }

// int main()
// {
//     try
//     {
//         const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
//         const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
//         const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
//         const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

//         Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::DirectX);

//         InitResources(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

//         while (window->IsRunning())
//         {
//             window->BeginFrame();
//                 static float theta{0}; theta += 0.05f;
//                 DX::GetRenderTargetView()->Bind();
//                 DX::GetRenderTargetView()->Clear();
//                 RenderMesh(0, theta, characterMesh.indices.size());
//             window->EndFrame();
//         }
//     }
//     catch (const std::runtime_error& exception)
//     {
//         MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
//     }

//     return 0;
// }