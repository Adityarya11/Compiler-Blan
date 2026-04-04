cmake_minimum_required(VERSION 3.15)
project(blan_compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(blan_core
    src/lexer/lexer.cc
    src/lexer/token.cc
    src/parser/parser.cc
    src/interpreter/evaluator.cc
)

target_include_directories(blan_core PUBLIC include)

add_executable(blan
  src/main.cc
)

target_link_libraries(blan PRIVATE blan_core)

# Enable Testing
enable_testing()

# Enable GTests
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0 
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(compilerTests
    tests/LexerIntegrationTests.cc
)

target_link_libraries(compilerTests PRIVATE blan_core GTest::gtest_main)

# Tell CMake to discover and run these tests
include(GoogleTest)
gtest_discover_tests(compilerTests)