cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(Shared)

set(Shared_MAJOR 1)
set(Shared_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Sources/Shared/Geometry/Direction.cpp
        Sources/Shared/Geometry/Direction.hpp
        Sources/Shared/Geometry/Vec2.hpp
        Sources/Shared/Command.cpp
        Sources/Shared/Command.hpp
        Sources/Shared/Geometry.hpp
        Sources/Shared/Global.hpp
        Sources/Shared/Log.cpp
        Sources/Shared/Log.hpp
        Sources/Shared/Math.hpp
        Sources/Shared/ThreadSafeQueue.hpp
        Sources/Shared/TileGrid_Info.hpp
        Sources/Shared/Types.hpp)

set(LIBRARY_NAME "Shared")
add_library(${LIBRARY_NAME} ${SOURCE_FILES})

include_directories(Sources)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
find_package(SFML 2 REQUIRED system window graphics network) #audio
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES})
endif()