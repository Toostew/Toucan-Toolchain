cmake_minimum_required(VERSION 4.0)
project(NTR-toolchain)
set(CMAKE_CXX_STANDARD 17) #
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(parser src/main.cpp)

target_include_directories(parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
