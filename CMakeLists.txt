cmake_minimum_required(VERSION 3.18)

project(PROJECT_CPP LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23) # si nécessaire

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file (GLOB_RECURSE sourceCode src/*.cpp src/*.h player.cpp)

qt_standard_project_setup()

add_executable(PROJECT_CPP ${sourceCode})

target_link_libraries(PROJECT_CPP PRIVATE Qt6::Widgets)