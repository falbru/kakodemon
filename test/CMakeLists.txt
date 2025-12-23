add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/Catch2 ${CMAKE_BINARY_DIR}/vendor/Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/trompeloeil ${CMAKE_BINARY_DIR}/vendor/trompeloeil EXCLUDE_FROM_ALL)

add_executable(tests
    unit/test_glyphlines.cpp
    unit/test_utf8string.cpp
    integration/test_localsession.cpp
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
    ${CMAKE_SOURCE_DIR}/src/adapters/kakoune/remotesession.cpp
    ${CMAKE_SOURCE_DIR}/src/application/model/kakouneclient.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/alignment.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/atom.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/color.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/face.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/geometry.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/line.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/lines.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/menu.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/modeline.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/ports/font.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/ports/kakouneinterface.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/statusline.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/utf8string.cpp
)

target_include_directories(tests PRIVATE mocks/ ${CMAKE_SOURCE_DIR}/src/)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain trompeloeil::trompeloeil nlohmann_json::nlohmann_json spdlog::spdlog)

include(CTest)
include(Catch)
catch_discover_tests(tests)
