cmake_minimum_required(VERSION 3.20)
project(AkinatorProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === БИБЛИОТЕКА ===
# Указываем пути к файлам в папке src/
add_library(akinator_lib
        src/Node.cpp
        src/QuestionNode.cpp
        src/AnswerNode.cpp
        src/KnowledgeTree.cpp
        src/Benchmark.cpp

)

# Где искать заголовочные файлы
target_include_directories(akinator_lib PUBLIC include)

# === ОСНОВНАЯ ПРОГРАММА ===
add_executable(akinator main.cpp)
target_link_libraries(akinator PRIVATE akinator_lib)

# === ТЕСТЫ ===
enable_testing()
find_package(GTest REQUIRED)

find_package(nlohmann_json REQUIRED)
target_link_libraries(akinator_lib PRIVATE nlohmann_json::nlohmann_json)

# ПРОВЕРЬТЕ: есть ли папка tests/ и файл TestNode.cpp?
# Если нет, создайте их или измените путь
add_executable(akinator_tests
        tests/test_entity_searcher.cpp
        tests/test_tree_serializer.cpp
        # Если TestNode.cpp включает другие файлы, они уже в библиотеке
)

target_link_libraries(akinator_tests PRIVATE
        akinator_lib
        GTest::gtest
        GTest::gtest_main
)

target_include_directories(akinator_tests PRIVATE
        include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Регистрируем тесты
add_test(NAME akinator_tests COMMAND akinator_tests)