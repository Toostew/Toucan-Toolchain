add_executable(lexer src/TokenTable.cpp)

target_include_directories(lexer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

