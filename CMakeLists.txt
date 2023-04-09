cmake_minimum_required(VERSION 3.23)
project(ConsoleEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(ConsoleEngine main.cpp)

target_compile_options(ConsoleEngine PRIVATE -DUNICODE)
