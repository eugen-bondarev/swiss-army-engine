cmake_minimum_required(VERSION 3.16)

project(direct3d VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE VAR_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${VAR_SRC})

link_directories(
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64"
)

target_link_libraries(${PROJECT_NAME} 
    d3d11
    dxgi
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx11.lib"
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx10.lib"
    dxguid
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)