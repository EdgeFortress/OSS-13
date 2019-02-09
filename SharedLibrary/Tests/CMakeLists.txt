cmake_minimum_required(VERSION 3.6)

project(Shared_Tests)

file(GLOB_RECURSE SOURCE_FILES Sources/*.cpp)

set(EXECUTABLE_NAME "Shared_Tests")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(${EXECUTABLE_NAME} ${GTEST_LIBRARIES} pthread Shared)