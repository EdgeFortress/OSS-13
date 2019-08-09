cmake_minimum_required(VERSION 3.6)

project(OSS13-Server)

include(FindPkgConfig)

pkg_search_module(PYTHON REQUIRED python3)

file(GLOB_RECURSE SOURCE_FILES Sources/*.cpp)

set(EXECUTABLE_NAME "OSS13-Server")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} Shared)
target_link_libraries(${EXECUTABLE_NAME} pthread)
target_link_libraries(${EXECUTABLE_NAME} ${PYTHON_LIBRARIES})

include_directories(Sources)
include_directories(Include)
include_directories("${CMAKE_SOURCE_DIR}/SharedLibrary/Sources")
include_directories(../External/pybind11/include)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(SFML REQUIRED system window graphics network) #audio

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network)
