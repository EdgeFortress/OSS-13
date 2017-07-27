cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(EdgeFortress)

set(EdgeFortress_VERSION_MAJOR 1)
set(EdgeFortress_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

#set(EXECUTABLE_NAME "EdgeFortress")
#add_executable(EdgeFortress ${SOURCE_FILES})

add_subdirectory("SharedLibrary")
add_subdirectory("GasProject Client")
add_subdirectory("GasProject Server")