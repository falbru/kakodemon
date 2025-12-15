add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/Catch2 ${CMAKE_BINARY_DIR}/vendor/Catch2 EXCLUDE_FROM_ALL)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
