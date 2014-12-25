#
# Copyright (C) 2014, Yeolar <yeolar@gmail.com>
#

project(CORAL)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_MICRO "0")
set(VERSION_PATCH "0")

cmake_minimum_required(VERSION 2.6.0)

enable_testing()

add_definitions(-Wall)

set(BUILD_SHARED_LIBS ON)

find_package(Threads REQUIRED)
message(STATUS "Use thread library: ${CMAKE_THREAD_LIBS_INIT}")
find_package(GTest REQUIRED)

set(LLVM "/usr")

if (LLVM)
    message(STATUS "Using llvm: ${LLVM}")
    set(CMAKE_C_COMPILER "${LLVM}/bin/clang")
    set(CMAKE_C_FLAGS "-std=c99")
    set(CMAKE_C_FLAGS_DEBUG "-g -v")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_COMPILER "${LLVM}/bin/clang++")
    set(CMAKE_CXX_FLAGS "-std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -v")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

include_directories(.)

add_subdirectory(coral)
