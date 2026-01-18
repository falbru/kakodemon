add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/Catch2 ${CMAKE_BINARY_DIR}/vendor/Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/trompeloeil ${CMAKE_BINARY_DIR}/vendor/trompeloeil EXCLUDE_FROM_ALL)

add_executable(tests
    unit/test_glyphlines.cpp
    unit/test_glyphlinesbuilder.cpp
    unit/test_utf8string.cpp
    unit/test_cliparser.cpp
    unit/test_line.cpp
    unit/test_color.cpp
    integration/test_fontconfigresolver.cpp
    integration/test_localsession.cpp
    integration/test_kakouneclientprocess.cpp
    integration/test_namedpipecommandinterface.cpp
    ${CMAKE_SOURCE_DIR}/src/application/cliparser.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/fontconfig/fontconfigresolver.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/atom.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/color.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/coord.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/face.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/infostyle.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/jsonrpckakouneinterface.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/kakouneclientprocess.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/kakouneframestatemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/line.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/menustyle.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/localsession.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/namedpipe/namedpipecommandinterface.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/remotesession.cpp
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/uioptions.cpp
    ${CMAKE_SOURCE_DIR}/src/application/model/kakouneclient.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/alignment.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/atom.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/color.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/face.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/fontmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/geometry.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/glyphatom.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/glyphline.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/glyphlines.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/glyphlinesbuilder.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/line.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/lines.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/menu.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/modeline.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/ports/font.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/ports/kakouneinterface.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/statusline.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/utf8string.cpp
)

target_include_directories(tests PRIVATE mocks/ ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_BINARY_DIR})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain trompeloeil::trompeloeil nlohmann_json::nlohmann_json spdlog::spdlog Fontconfig::Fontconfig)

include(CTest)
include(Catch)
catch_discover_tests(tests)
