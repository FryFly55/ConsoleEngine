cmake_minimum_required(VERSION 3.23)
project(ConsoleEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static -Wall")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
file (GLOB TEST-SOURCES "TestSrc/*.cpp")

add_executable(ConsoleEngine ${SOURCES})
# add_executable(ConsoleEngine ${TEST-SOURCES})

target_compile_options(ConsoleEngine PRIVATE -DUNICODE)
