cmake_minimum_required(VERSION 3.6)

project(EdgeFortress)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("SharedLibrary")
add_subdirectory("GasProject Client")
add_subdirectory("GasProject Server")
