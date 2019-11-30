cmake_minimum_required(VERSION 3.15)
project(StorageBufferaManager)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

add_executable(StorageBufferaManager main.cc buffer/Frame.h buffer/Buffer.h buffer/BMgr.h storage/DSMgr.cc storage/DSMgr.h include/env.h)
target_link_libraries(StorageBufferaManager ${GTEST_LIBRARIES} pthread)
