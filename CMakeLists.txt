cmake_minimum_required(VERSION 3.10)
project(kakodemon VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(src/config.hpp.in config.hpp)

option(BUILD_TESTS "Build the test suite" OFF)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(vendor/glfw)

add_subdirectory(vendor/glm)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(vendor/nlohmann_json)

find_package(Freetype)

find_package(Fontconfig)

add_subdirectory(vendor/spdlog)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/)
endif()

set(SRC
    src/adapters/fontconfig/fontconfigresolver.cpp
    src/adapters/freetype/freetypefontengine.cpp
    src/adapters/freetype/freetypelibrary.cpp
    src/adapters/kakoune/atom.cpp
    src/adapters/kakoune/color.cpp
    src/adapters/kakoune/coord.cpp
    src/adapters/kakoune/face.cpp
    src/adapters/kakoune/infostyle.cpp
    src/adapters/kakoune/infostyle.cpp
    src/adapters/kakoune/jsonrpckakouneinterface.cpp
    src/adapters/kakoune/kakouneclientprocess.cpp
    src/adapters/kakoune/kakouneframestatemanager.cpp
    src/adapters/kakoune/line.cpp
    src/adapters/kakoune/localsession.cpp
    src/adapters/kakoune/menustyle.cpp
    src/adapters/kakoune/remotesession.cpp
    src/adapters/kakoune/uioptions.cpp
    src/adapters/opengl/application.cpp
    src/adapters/opengl/font.cpp
    src/adapters/opengl/renderer.cpp
    src/adapters/opengl/shaderprogram.cpp
    src/application/application.cpp
    src/application/applicationbuilder.cpp
    src/application/cliparser.cpp
    src/application/controller/editorcontroller.cpp
    src/application/controller/infoboxcontroller.cpp
    src/application/controller/inputcontroller.cpp
    src/application/controller/menucontroller.cpp
    src/application/controller/mousecontroller.cpp
    src/application/model/kakouneclient.cpp
    src/application/view/glyphsequence.cpp
    src/application/view/infobox.cpp
    src/application/view/inlinemenu.cpp
    src/application/view/kakounecontentview.cpp
    src/application/view/layoutmanager.cpp
    src/application/view/promptmenu.cpp
    src/application/view/searchmenu.cpp
    src/application/view/statusbar.cpp
    src/application/view/widgets/input.cpp
    src/application/view/widgets/scrollbar.cpp
    src/application/view/widgets/scrolledmenuitems.cpp
    src/domain/alignment.cpp
    src/domain/atom.cpp
    src/domain/color.cpp
    src/domain/face.cpp
    src/domain/fontmanager.cpp
    src/domain/geometry.cpp
    src/domain/glyphatom.cpp
    src/domain/glyphline.cpp
    src/domain/glyphlines.cpp
    src/domain/glyphlinesbuilder.cpp
    src/domain/line.cpp
    src/domain/lines.cpp
    src/domain/menu.cpp
    src/domain/modeline.cpp
    src/domain/ports/font.cpp
    src/domain/ports/kakouneinterface.cpp
    src/domain/statusline.cpp
    src/domain/utf8string.cpp
    src/main.cpp
    vendor/glad/src/glad.c
)

add_executable(kakod ${SRC})

target_include_directories(kakod PRIVATE src/ vendor/glad/include ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(kakod glfw glm::glm nlohmann_json::nlohmann_json Freetype::Freetype Fontconfig::Fontconfig spdlog::spdlog)
