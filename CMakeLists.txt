cmake_minimum_required(VERSION 3.10)
project(StorageBufferManager)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra -Wpedantic -g)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(StorageBufferManager main.cc storage/DSMgr.cc buffer/BMgr.cc)

add_executable(Test_DSMgr tests/test_DSMgr.cc storage/DSMgr.cc)
target_link_libraries(Test_DSMgr ${GTEST_LIBRARIES} pthread)

add_executable(Test_BMgr tests/test_BMgr.cc buffer/BMgr.cc storage/DSMgr.cc buffer/Buffer.h)
target_link_libraries(Test_BMgr ${GTEST_LIBRARIES} pthread)

add_executable(Test_Total tests/test_Total.cc buffer/BMgr.cc storage/DSMgr.cc)
target_link_libraries(Test_Total ${GTEST_LIBRARIES} pthread)
