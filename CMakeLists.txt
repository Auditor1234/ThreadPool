cmake_minimum_required(VERSION 3.5)

project(ThreadPool)

set(CXX_FLAGS
    -Wall
    -std=c++11
    -lpthread
    )

set(SOURCE_FILES
    src/main.cpp
    src/thread_pool.cpp
    )

add_executable(ThreadPool ${SOURCE_FILES})