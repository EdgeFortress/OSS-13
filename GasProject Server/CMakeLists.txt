cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(GasProject_Server)

set(GasProject_Server_VERSION_MAJOR 1)
set(GasProject_Server_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Sources/Database/UsersDB.cpp
        Sources/Database/UsersDB.hpp
        Sources/Network/Connection.hpp
        Sources/Network/Differences.cpp
        Sources/Network/Differences.hpp
        Sources/Network/NetworkController.cpp
        Sources/Network/NetworkController.hpp
        Sources/Network/Packing.cpp
        Sources/World/Objects/Component.cpp
        Sources/World/Objects/Component.hpp
        Sources/World/Objects/Control.cpp
        Sources/World/Objects/Control.hpp
        Sources/World/Objects/Creature.cpp
        Sources/World/Objects/Creature.hpp
        Sources/World/Objects/Ghost.cpp
        Sources/World/Objects/Ghost.hpp
        Sources/World/Objects/Human.cpp
        Sources/World/Objects/Human.hpp
        Sources/World/Objects/Item.cpp
        Sources/World/Objects/Item.hpp
        Sources/World/Objects/Object.cpp
        Sources/World/Objects/Object.hpp
        Sources/World/Objects/Turf.cpp
        Sources/World/Objects/Turf.hpp
        Sources/World/Camera.cpp
        Sources/World/Camera.hpp
        Sources/World/Objects.hpp
        Sources/World/World.cpp
        Sources/World/World.hpp
        Sources/Global.hpp
        Sources/Player.cpp
        Sources/Player.hpp
        Sources/PlayerCommand.cpp
        Sources/PlayerCommand.hpp
        Sources/Server.cpp
        Sources/Server.hpp)

set(EXECUTABLE_NAME "GasProject_Server")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/cmake-build-debug/SharedLibrary/libShared.a)
target_link_libraries(${EXECUTABLE_NAME} -lpthread)

include_directories(Sources)
include_directories("${CMAKE_SOURCE_DIR}/SharedLibrary/Sources")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
find_package(SFML 2 REQUIRED system window graphics network) #audio
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()