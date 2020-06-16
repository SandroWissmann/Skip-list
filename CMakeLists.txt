cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(skip_list)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(
    ${CMAKE_SOURCE_DIR}/include 
)

add_subdirectory(thirdparty/googletest)

add_executable(test 
    test/skip_list_test.cpp
)

target_link_libraries(test 
    gtest_main 
)

