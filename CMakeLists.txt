cmake_minimum_required(VERSION 3.15)
project(TradeSimPP)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main application
add_library(tradesim_lib ${SOURCES})
target_include_directories(tradesim_lib PUBLIC include)

add_executable(tradesim src/main.cpp)  # Or whichever main you have
target_link_libraries(tradesim tradesim_lib)

# GoogleTest
enable_testing()
add_subdirectory(tests/googletest)
include_directories(tests/googletest/googletest/include)

add_executable(runTests tests/test_main.cpp tests/test_order.cpp)
target_link_libraries(runTests tradesim_lib gtest gtest_main pthread)

add_test(NAME RunTests COMMAND runTests)
