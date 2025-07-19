cmake_minimum_required(VERSION 3.10)
project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(vendor/glfw)

add_subdirectory(vendor/glm)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(vendor/nlohmann_json)

find_package(Freetype)

add_subdirectory(vendor/spdlog)

set(SRC
    src/main.cpp
    src/application.cpp
    src/controller/editorcontroller.cpp
    src/controller/inputcontroller.cpp
    src/model/kakouneclient.cpp
    src/model/kakouneclientprocess.cpp
    src/model/line.cpp
    src/model/atom.cpp
    src/model/face.cpp
    src/view/kakounecontentview.cpp
    vendor/glad/src/glad.c
)

add_executable(editor ${SRC})

target_include_directories(editor PRIVATE src/ vendor/glad/include)
target_link_libraries(editor glfw glm::glm nlohmann_json::nlohmann_json Freetype::Freetype spdlog::spdlog)
