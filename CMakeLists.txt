cmake_minimum_required(VERSION 3.15)
project(TradeSimPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Main library ===
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(tradesim_lib ${SOURCES})
target_include_directories(tradesim_lib PUBLIC include)

# === Main executable ===
# add_executable(tradesim src/main.cpp)  # replace with actual main file if different
# target_link_libraries(tradesim tradesim_lib)

# === GoogleTest ===
enable_testing()
add_subdirectory(tests/googletest)

# === Test executable ===
file(GLOB TEST_SOURCES "tests/*.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp")
add_executable(runTests tests/test_main.cpp ${TEST_SOURCES})
target_link_libraries(runTests tradesim_lib gtest gtest_main pthread)

add_test(NAME RunTests COMMAND runTests)
