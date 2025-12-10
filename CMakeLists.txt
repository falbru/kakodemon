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

find_package(Fontconfig)

add_subdirectory(vendor/spdlog)

set(SRC
    src/main.cpp
    src/application.cpp
    src/font.cpp
    src/fontconfigresolver.cpp
    src/freetypefontengine.cpp
    src/fontconfigresolver.cpp
    src/controller/editorcontroller.cpp
    src/controller/inputcontroller.cpp
    src/controller/menucontroller.cpp
    src/controller/infoboxcontroller.cpp
    src/view/kakounecontentview.cpp
    src/view/statusbar.cpp
    src/view/promptmenu.cpp
    src/view/inlinemenu.cpp
    src/view/infobox.cpp
    src/view/layoutmanager.cpp
    src/view/glyphsequence.cpp
    src/view/widgets/input.cpp
    src/view/widgets/scrollbar.cpp
    src/view/widgets/scrolledmenuitems.cpp
    src/core/alignment.cpp
    src/core/utf8string.cpp
    src/kakoune/kakouneframestatemanager.cpp
    src/kakoune/kakouneclientprocess.cpp
    src/kakoune/coord.cpp
    src/kakoune/menustyle.cpp
    src/kakoune/infostyle.cpp
    src/kakoune/line.cpp
    src/kakoune/atom.cpp
    src/kakoune/face.cpp
    src/kakoune/color.cpp
    src/opengl/application.cpp
    src/opengl/renderer.cpp
    src/opengl/shaderprogram.cpp
    src/opengl/font.cpp
    vendor/glad/src/glad.c
)

add_executable(kakod ${SRC})

target_include_directories(kakod PRIVATE src/ vendor/glad/include)
target_link_libraries(kakod glfw glm::glm nlohmann_json::nlohmann_json Freetype::Freetype Fontconfig::Fontconfig spdlog::spdlog)
