cmake_minimum_required(VERSION 3.16)

project(swiss-army-engine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE VAR_SRC Source/*.cpp external/imgui/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    # WIN32 
    ${VAR_SRC}
)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

link_directories(
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64"
)

add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

target_link_libraries(${PROJECT_NAME} 
    dxgi
    d3d10
    d3d11
    dxguid
    d3dcompiler
    
    ${Vulkan_LIBRARIES}

    assimp
    glfw
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um"

	${Vulkan_INCLUDE_DIR} 
    
    "external/glfw/include"
    "external/glm/glm"
    "external/assimp/include"
    "external/stb_image"
    "external/imgui"
)