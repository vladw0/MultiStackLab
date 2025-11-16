cmake_minimum_required(VERSION 3.14)
project(MultiStackLab LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(mslib INTERFACE)
target_include_directories(mslib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(multistack src/main.cpp)
target_link_libraries(multistack PRIVATE mslib)

add_subdirectory(gtest)

add_executable(multistack_test test/test_multistack.cpp)
target_link_libraries(multistack_test PRIVATE mslib gtest_main)
include(GoogleTest)
gtest_discover_tests(multistack_test)
