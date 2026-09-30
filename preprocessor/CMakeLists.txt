
add_executable(preprocessor
        src/preprocess.cpp
        inc/preprocess.h
        src/main.cpp
        inc/main.h)



target_include_directories(preprocessor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

