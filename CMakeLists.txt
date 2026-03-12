cmake_minimum_required(VERSION 3.20)
project(Akinator VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем стандарт C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции сборки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ========== Поиск зависимостей ==========
# Ищем nlohmann_json (можно также использовать FetchContent или установить через vcpkg/conan)
find_package(nlohmann_json REQUIRED)
if(NOT nlohmann_json_FOUND)
    message(FATAL_ERROR "nlohmann_json not found. Install it or use FetchContent.")
endif()

# Google Test будем подключать через FetchContent (рекомендуемый способ)
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# ========== Основная библиотека (общие классы) ==========
# Собираем все исходники, кроме main (если main будет в отдельном исполняемом файле)
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "src/*.cc")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")  # если main.cpp лежит в src

add_library(akinator_lib STATIC ${LIB_SOURCES})

target_include_directories(akinator_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}  # если какие-то заголовки прямо в src
)

target_link_libraries(akinator_lib PUBLIC
        nlohmann_json::nlohmann_json
)

# ========== Исполняемый файл игры ==========
add_executable(akinator main.cpp)  # путь к main.cpp
target_link_libraries(akinator PRIVATE akinator_lib)

# ========== Тесты ==========
enable_testing()

# Собираем все тестовые файлы
file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name}
            PRIVATE
            akinator_lib
            GTest::gtest_main
            GTest::gmock
    )
    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Если хотите один тестовый таргет со всеми тестами (альтернатива):
# add_executable(tests ${TEST_SOURCES})
# target_link_libraries(tests akinator_lib GTest::gtest_main GTest::gmock)
# add_test(NAME AllTests COMMAND tests)

# ========== Установка (опционально) ==========
# install(TARGETS akinator akinator_lib RUNTIME DESTINATION bin LIBRARY DESTINATION lib)