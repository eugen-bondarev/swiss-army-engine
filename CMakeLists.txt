cmake_minimum_required(VERSION 3.16)

project(direct3d VERSION 1.0.0)

file(GLOB_RECURSE VAR_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${VAR_SRC})