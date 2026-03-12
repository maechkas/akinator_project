cmake_minimum_required(VERSION 3.16)
project(akinator_project)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

enable_testing()

# Библиотека
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(akinator_lib ${SOURCES})
target_include_directories(akinator_lib PRIVATE include)

# JSON
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    target_link_libraries(akinator_lib nlohmann_json::nlohmann_json)
endif()
# ========== ТЕСТЫ ==========
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE
        gtest_main
        akinator_lib
)
target_include_directories(run_tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(run_tests)