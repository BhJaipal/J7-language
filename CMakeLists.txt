cmake_minimum_required(VERSION 3.28)
project("J7 programming language" LANGUAGES CXX)
message("Building ${CMAKE_PROJECT_NAME} in ${CMAKE_SYSTEM_NAME} system")
set(COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(Sources "src/utils/Errors.cpp" "src/utils/Find.cpp" "src/Lexer.cpp")
include_directories(include)

add_executable(j7.exe main.cpp ${Sources})
