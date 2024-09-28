cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_C_COMPILER "/bin/gcc")
set(CMAKE_CXX_COMPILER "/bin/g++")
project(J7)


add_executable(j7 "main/main.cpp" "src/lexer.cpp" "src/utils/Errors.cpp" "src/utils/Find.cpp")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
include_directories("include")