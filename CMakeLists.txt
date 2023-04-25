cmake_minimum_required(VERSION 3.23)
project(ConsoleEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static -Wall")

include_directories(include editorInclude testInclude)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TEST-SOURCES "TestSrc/*.cpp")
file(GLOB_RECURSE EDITOR-SOURCES "editorSrc/*.cpp")

add_executable(ConsoleEngine ${SOURCES})
target_compile_options(ConsoleEngine PRIVATE -DUNICODE)

add_executable(Editor ${EDITOR-SOURCES})
target_compile_options(Editor PRIVATE -DUNICODE)

add_executable(Testing ${TEST-SOURCES})
target_compile_options(Editor PRIVATE -DUNICODE)
