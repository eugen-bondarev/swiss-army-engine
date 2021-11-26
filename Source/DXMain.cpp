// #include "Window/RawWindow.h"
// #include "Common/Vertex.h"
// #include "Util/Assets.h"
// #include "API/Sampler.h"
// #include "API/Window.h"
// #include "API/Buffer.h"
// #include "API/Shader.h"
// #include "DX/DX.h"

// static Ptr<DX::MappableConstantBuffer> sceneConstantBuffer{nullptr};
// static DX::XMMATRIX                    projectionMatrix;
// static Ptr<API::Shader>                shader{nullptr};
// static Ptr<API::Sampler>               sampler{nullptr};

// struct Mesh
// {
//     Ptr<DX::VertexBuffer> vertexBuffer;
//     Ptr<DX::IndexBuffer> indexBuffer;
//     Ptr<DX::Texture> texture;

//     Mesh(const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture) : numIndices {static_cast<unsigned int>(characterMesh.indices.size())}
//     {
//         vertexBuffer = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
//         indexBuffer = CreatePtr<DX::IndexBuffer>(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
//         texture = CreatePtr<DX::Texture>(characterTexture.size, characterTexture.data);
//         ApplyTransform();
//     }

//     void Render()
//     {
//         vertexBuffer->Bind();
//         indexBuffer->Bind();
//         texture->Bind();

//         DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(sceneConstantBuffer->Map());
//             *data = GetSpace();
//         sceneConstantBuffer->Unmap();

//         DX_TRY(DX::GetDevice().GetDxContext().DrawIndexed(GetNumIndices(), 0u, 0u));
//     }

//     void ApplyTransform()
//     {
//         modelMatrix = DX::XMMatrixRotationX(-rotation.x) *
//             DX::XMMatrixRotationY(-rotation.y + DX::XM_PI /* weird fix */) *
//             DX::XMMatrixRotationZ(-rotation.z) *
//             DX::XMMatrixTranslation(position.x, position.y, -position.z);

//         space = DX::XMMatrixTranspose(
//             modelMatrix *
//             projectionMatrix
//         );
//     }

//     template <typename... Args>
//     void SetPosition(Args&&... args) { position = Vec3f(std::forward<Args>(args)...); ApplyTransform(); }
//     void SetPositionX(const float x) { position.x = x; ApplyTransform(); }
//     void SetPositionY(const float y) { position.y = y; ApplyTransform(); }
//     void SetPositionZ(const float z) { position.z = z; ApplyTransform(); }

//     template <typename... Args>
//     void SetRotation(Args&&... args) { rotation = Vec3f(std::forward<Args>(args)...); ApplyTransform(); }
//     void SetRotationX(const float x) { rotation.x = x; ApplyTransform(); }
//     void SetRotationY(const float y) { rotation.y = y; ApplyTransform(); }
//     void SetRotationZ(const float z) { rotation.z = z; ApplyTransform(); }

//     const DX::XMMATRIX& GetSpace() const
//     {
//         return space;
//     }

//     unsigned int GetNumIndices() const
//     {
//         return numIndices;
//     }

// private:
//     DX::XMMATRIX modelMatrix;
//     DX::XMMATRIX space;

//     Vec3f position {0};
//     Vec3f rotation {0};

//     unsigned int numIndices;
// };

// void InitResources(const Util::TextAsset& vsCode, const Util::TextAsset& psCode, const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture)
// {
//     projectionMatrix = DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, DX::GetSwapChain()->GetAspectRatio(), 0.1f, 1000.0f);
//     sceneConstantBuffer = CreatePtr<DX::MappableConstantBuffer>(sizeof(DX::XMMATRIX), 0, nullptr);
//     shader = API::Shader::Create(vsCode, psCode);
//     sampler = API::Sampler::Create();
//     DX::GetDevice().GetDxContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// }

// int main()
// {
//     try
//     {
//         const Util::TextAsset vertexShaderCode = Util::LoadTextFile("Assets/Shaders/VertexShader.hlsl");
//         const Util::TextAsset pixelShaderCode = Util::LoadTextFile("Assets/Shaders/PixelShader.hlsl");
//         const Util::ModelAsset characterMesh = Util::LoadModelFile("Assets/Models/CharacterModel.fbx");
//         const Util::ImageAsset characterTexture = Util::LoadImageFile("Assets/Images/CharacterTexture.png");

//         Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::DirectX, WindowMode::Windowed, false, Vec2ui {1024, 768});

//         InitResources(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

//         std::vector<Mesh> meshes;

//         meshes.emplace_back(characterMesh, characterTexture);
//         meshes.emplace_back(characterMesh, characterTexture);
//         meshes.emplace_back(characterMesh, characterTexture);
//         meshes.emplace_back(characterMesh, characterTexture);

//         meshes[0].SetPosition(-5, -5, -15);
//         meshes[1].SetPosition(-5, -5, -25);
//         meshes[2].SetPosition( 5, -5, -15);
//         meshes[3].SetPosition( 5, -5, -25);

//         sceneConstantBuffer->Bind();
//         sampler->Bind();
//         shader->Bind();

//         while (window->IsRunning())
//         {
//             window->BeginFrame();

//                 const float deltaTime {window->GetDeltaTime()};
//                 static float timer {0}; timer += deltaTime;
//                 static unsigned int fpsCounter {0};
//                 static unsigned int numFpsRecordings {0};
//                 static unsigned int averageFps {0};

//                 if (timer >= 1.0f)
//                 {
//                     fpsCounter += 1.0f / deltaTime;
//                     numFpsRecordings += 1;

//                     averageFps = static_cast<unsigned int>(static_cast<float>(fpsCounter) / static_cast<float>(numFpsRecordings));
//                     VAR_OUT(averageFps);

//                     timer = 0;
//                 }

//                 static float theta{0}; theta += deltaTime;

//                 DX::GetRenderTargetView()->Bind();
//                 DX::GetRenderTargetView()->Clear();

//                 for (size_t i = 0; i < meshes.size(); ++i)
//                 {
//                     meshes[i].SetRotationY(theta);
//                     meshes[i].Render();
//                 }

//             window->EndFrame();
//         }
//     }
//     catch (const std::runtime_error& exception)
//     {
//         MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
//     }

//     return 0;
// }