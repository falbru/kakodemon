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
    src/application/application.cpp
    src/domain/ports/font.cpp
    src/adapters/fontconfig/fontconfigresolver.cpp
    src/adapters/freetype/freetypefontengine.cpp
    src/application/controller/editorcontroller.cpp
    src/application/controller/inputcontroller.cpp
    src/application/controller/mousecontroller.cpp
    src/application/controller/menucontroller.cpp
    src/application/controller/infoboxcontroller.cpp
    src/application/view/kakounecontentview.cpp
    src/application/view/statusbar.cpp
    src/application/view/promptmenu.cpp
    src/application/view/inlinemenu.cpp
    src/application/view/searchmenu.cpp
    src/application/view/infobox.cpp
    src/application/view/layoutmanager.cpp
    src/application/view/glyphsequence.cpp
    src/application/view/widgets/input.cpp
    src/application/view/widgets/scrollbar.cpp
    src/application/view/widgets/scrolledmenuitems.cpp
    src/domain/alignment.cpp
    src/domain/utf8string.cpp
    src/kakoune/kakouneframestatemanager.cpp
    src/kakoune/kakouneclientprocess.cpp
    src/kakoune/coord.cpp
    src/kakoune/menustyle.cpp
    src/kakoune/infostyle.cpp
    src/kakoune/line.cpp
    src/kakoune/atom.cpp
    src/kakoune/face.cpp
    src/kakoune/color.cpp
    src/adapters/opengl/application.cpp
    src/adapters/opengl/renderer.cpp
    src/adapters/opengl/shaderprogram.cpp
    src/adapters/opengl/font.cpp
    vendor/glad/src/glad.c
)

add_executable(kakod ${SRC})

target_include_directories(kakod PRIVATE src/ vendor/glad/include)
target_link_libraries(kakod glfw glm::glm nlohmann_json::nlohmann_json Freetype::Freetype Fontconfig::Fontconfig spdlog::spdlog)
