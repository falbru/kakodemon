cmake_minimum_required(VERSION 3.10)
project(kakodemon)

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
    src/view/kakounecontentview.cpp
    src/kakoune/kakouneframestatemanager.cpp
    src/kakoune/kakouneclientprocess.cpp
    src/kakoune/line.cpp
    src/kakoune/atom.cpp
    src/kakoune/face.cpp
    vendor/glad/src/glad.c
)

add_executable(kakod ${SRC})

target_include_directories(kakod PRIVATE src/ vendor/glad/include)
target_link_libraries(kakod glfw glm::glm nlohmann_json::nlohmann_json Freetype::Freetype spdlog::spdlog)
