cmake_minimum_required(VERSION 3.6)

project(GasProject_Server)

include(FindPkgConfig)

pkg_search_module(LUA REQUIRED luajit lua-5.1-c++)

file(GLOB_RECURSE SOURCE_FILES Sources/*.cpp)

set(EXECUTABLE_NAME "GasProject_Server")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} Shared)
target_link_libraries(${EXECUTABLE_NAME} pthread)
target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})

include_directories(Sources)
include_directories("${CMAKE_SOURCE_DIR}/SharedLibrary/Sources")
include_directories(${LUA_INCLUDE_DIRS})

find_package(SFML REQUIRED system window graphics network) #audio

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network)
