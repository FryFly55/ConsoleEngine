cmake_minimum_required(VERSION 3.23)
project(ConsoleEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Set the path to the directory containing libglfw3.a
# todo: make this path relative, idk from where the commands are executed though
# on my laptop:
#set(GLFW_LIBRARY_DIR "C:/Users/fried/Documents/SavesExportsDaten/Programming/C++/ConsoleEngineGit/ConsoleEngine/lib")
# on my desktop:
set(GLFW_LIBRARY_DIR "C:/Users/FryFly55/SavesExportsDaten/Programmieren/C++/ConsoleEngineGit/ConsoleEngine/lib")

# Set the path to the include directory
set(INCLUDE_DIR "./include")

# set up include dir for the legacy version
set(OLD_INCLUDES "./legacyInclude")

# Collect all source files recursively from the ./src directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Test sources
file(GLOB_RECURSE TEST_SOURCES "test_src/*.cpp")

# old version, no shared files
file(GLOB_RECURSE OLD_SOURCES "old_src/*.cpp")

# First target is the main build
add_executable(ConsoleEngine ${SOURCE_FILES} src/glad.c main_src/main.cpp)

target_compile_options(ConsoleEngine PRIVATE -DUNICODE)

target_include_directories(ConsoleEngine PRIVATE ${INCLUDE_DIR})

target_link_libraries(ConsoleEngine PRIVATE "${GLFW_LIBRARY_DIR}/libglfw3.a")

# Second target is a test build for trying out features - only difference is entry point
# everything else is shared even if both may not use every source file
add_executable(TestEngine ${SOURCE_FILES} ${TEST_SOURCES} src/glad.c test_src/testMain.cpp)

target_compile_options(TestEngine PRIVATE -DUNICODE)

target_include_directories(TestEngine PRIVATE ${INCLUDE_DIR})

target_link_libraries(TestEngine PRIVATE "${GLFW_LIBRARY_DIR}/libglfw3.a")

# Third target is one of the oldest version of this rasterizer
add_executable(Old old_src/old.cpp)

target_compile_options(Old PRIVATE -DUNICODE)

target_include_directories(Old PRIVATE ${OLD_INCLUDES} ${INCLUDE_DIR})

# Test Target for the old script with OpenGL implementation
add_executable(OldGL old_src/oldGl.cpp src/glad.c)

target_compile_options(OldGL PRIVATE -DUNICODE)

target_include_directories(OldGL PRIVATE ${INCLUDE_DIR})

target_link_libraries(OldGL PRIVATE "${GLFW_LIBRARY_DIR}/libglfw3.a")
