cmake_minimum_required(VERSION 3.6)

project(Shared)

file(GLOB_RECURSE SOURCE_FILES Sources/*.cpp ../External/sfml-imgui/*.cpp)

set(LIBRARY_NAME "Shared")

add_library(${LIBRARY_NAME} ${SOURCE_FILES})

include_directories(Sources)

target_link_libraries(${LIBRARY_NAME} stdc++fs)
target_link_libraries(${LIBRARY_NAME} GL)

find_package(SFML REQUIRED system window graphics network) #audio

target_link_libraries(${LIBRARY_NAME} sfml-system sfml-graphics sfml-network)

add_subdirectory("Tests")