cmake_minimum_required(VERSION 3.10)
project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(vendor/glfw)

set(SRC
    src/main.cpp
    vendor/glad/src/glad.c
)

add_executable(editor ${SRC})

target_include_directories(editor PRIVATE vendor/glad/include)
target_link_libraries(editor glfw)
