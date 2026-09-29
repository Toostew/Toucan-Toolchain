cmake_minimum_required(VERSION 4.0)
project(NTR-toolchain)
set(CMAKE_CXX_STANDARD 17) #
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(lexer src/TokenTable.cpp src/main.cpp src/lexer.cpp)

target_include_directories(lexer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

