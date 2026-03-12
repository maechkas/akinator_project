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

# ТЕСТЫ
find_package(GTest REQUIRED)
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        akinator_lib
        nlohmann_json::nlohmann_json
)
target_include_directories(run_tests PRIVATE include)
# УБРАНО: gtest_discover_tests(run_tests) - оно крашит сборку!
