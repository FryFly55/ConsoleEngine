cmake_minimum_required(VERSION 3.23)
project(ConsoleEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(include)

file(GLOB_RECURSE Sources "src/*.cpp")


add_executable(ConsoleEngine ${Sources} src/Camera.cpp include/Camera.h)
target_compile_options(ConsoleEngine PRIVATE -DUNICODE)
