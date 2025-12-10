set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/build/generators/conan_toolchain.cmake)
cmake_minimum_required(VERSION 3.31)

project(game C)
set(CMAKE_C_STANDARD 17)
find_package(raylib REQUIRED)

add_library(snake src/snake.c src/snake.h)
target_link_libraries(snake raylib)

add_executable(game main.c)
target_link_libraries(game snake)
