cmake_minimum_required(VERSION 3.10)
project(runner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(include)

option(ENABLE_TESTS "Enable tests" ON)

add_executable(dr main.cpp)

if (ENABLE_TESTS)
    message(STATUS "Enabling tests")
    enable_testing()
    add_subdirectory(tests)
endif()
