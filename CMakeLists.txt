cmake_minimum_required(VERSION 3.15)
project(StorageBufferaManager)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

add_executable(StorageBufferaManager main.cpp include/Frame.h include/Buffer.h)
target_link_libraries(StorageBufferaManager ${GTEST_LIBRARIES} pthread)
