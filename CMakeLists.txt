cmake_minimum_required(VERSION 3.20)
project(AkinatorProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JSON
include(FetchContent)
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# GTest
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Библиотека
add_library(akinator_lib
        src/Node.cpp
        src/QuestionNode.cpp
        src/AnswerNode.cpp
        src/KnowledgeTree.cpp
        src/Benchmark.cpp
        src/EntitySearcher.cpp
        src/GameSession.cpp
        src/Player.cpp
        src/tree_serializer.cpp
        src/FileHandler.cpp
)

target_include_directories(akinator_lib PUBLIC include)
target_link_libraries(akinator_lib PRIVATE nlohmann_json::nlohmann_json)

# Основная программа
add_executable(akinator main.cpp)
target_link_libraries(akinator PRIVATE akinator_lib nlohmann_json::nlohmann_json)

# ТЕСТЫ — ТОЛЬКО ОДИН РАЗ!
enable_testing()
add_executable(akinator_tests
        tests/test_entity_searcher.cpp
        tests/test_tree_serializer.cpp
        tests/TestKnowledgeTree.cpp
        tests/TestNode.cpp
        tests/test_gamesession.cpp
        tests/test_player.cpp
        tests/test_filehandler.cpp

)
target_link_libraries(akinator_tests PRIVATE
        akinator_lib
        gtest
        gtest_main
        nlohmann_json::nlohmann_json
)
target_include_directories(akinator_tests PRIVATE include)
add_test(NAME akinator_tests COMMAND akinator_tests)
