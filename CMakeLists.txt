cmake_minimum_required(VERSION 3.16)

project(direct3d VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE VAR_SRC src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    # WIN32 
    ${VAR_SRC}
)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

link_directories(
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64"
)

add_subdirectory(external/glfw)

target_link_libraries(${PROJECT_NAME} 
    d3d11
    dxgi
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64/d3d11.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64/d3d10.lib"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64/d3dcompiler.lib"
    dxguid
    glfw
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um"
    "external/glfw/include"
    "external/stb_image"
    # "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)