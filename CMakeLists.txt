cmake_minimum_required(VERSION 3.15)
project(blan_compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(blan
    src/main.cpp
    src/lexer/lexer.cpp
    src/lexer/token.cpp
    src/parser/parser.cpp
    src/interpreter/evaluator.cpp
)

target_include_directories(blan PRIVATE include)
