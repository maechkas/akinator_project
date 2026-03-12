cmake_minimum_required(VERSION 3.21)
project(akinator_project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

enable_testing()

# ========== ОСНОВНАЯ БИБЛИОТЕКА ==========
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")

add_library(akinator_lib STATIC ${LIB_SOURCES})
target_include_directories(akinator_lib PUBLIC include)

# ========== JSON (АВТОМАТИЧЕСКАЯ ЗАГРУЗКА) ==========
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# Подключаем заголовочные файлы JSON к нашей библиотеке
target_include_directories(akinator_lib PRIVATE
        ${nlohmann_json_SOURCE_DIR}/include
)

message(STATUS "JSON library downloaded and connected from ${nlohmann_json_SOURCE_DIR}/include")

# ========== ИСПОЛНЯЕМЫЙ ФАЙЛ ИГРЫ ==========
add_executable(akinator_project main.cpp)
target_link_libraries(akinator_project PRIVATE akinator_lib)
target_include_directories(akinator_project PRIVATE include)

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