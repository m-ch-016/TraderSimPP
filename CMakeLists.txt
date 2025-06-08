cmake_minimum_required(VERSION 3.15)
project(TradeSimPP)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(tradesim ${SOURCES} ${HEADERS})

# GoogleTest
enable_testing()
add_subdirectory(tests/googletest)
include_directories(tests/googletest/googletest/include)

add_executable(runTests tests/test_main.cpp)
target_link_libraries(runTests gtest gtest_main)
add_test(NAME RunTests COMMAND runTests)
