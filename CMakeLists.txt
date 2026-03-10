cmake_minimum_required(VERSION 3.21)
project(akinator_project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(akinator_project
        main.cpp
        src/GameSession.cpp
        src/Player.cpp
)

target_include_directories(akinator_project PRIVATE include)