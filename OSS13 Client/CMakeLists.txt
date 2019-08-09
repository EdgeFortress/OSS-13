cmake_minimum_required(VERSION 3.6)

project(OSS13-Client)

file(GLOB_RECURSE SOURCE_FILES Sources/*.cpp)

set(EXECUTABLE_NAME "OSS13-Client")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} Shared)
target_link_libraries(${EXECUTABLE_NAME} pthread)

include_directories(Sources)
include_directories("${CMAKE_SOURCE_DIR}/SharedLibrary/Sources")

find_package(SFML COMPONENTS system window graphics network REQUIRED) #audio

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network)