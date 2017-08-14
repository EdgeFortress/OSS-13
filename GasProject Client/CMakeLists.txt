cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(GasProject_Client)

set(GasProject_Client_VERSION_MAJOR 1)
set(GasProject_Client_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Sources/Graphics/TileGrid/Object.cpp
        Sources/Graphics/TileGrid/Object.hpp
        Sources/Graphics/TileGrid/Tile.cpp
        Sources/Graphics/TileGrid/Tile.hpp
        Sources/Graphics/TileGrid/Block.cpp
        Sources/Graphics/TileGrid/Block.hpp
        Sources/Graphics/TileGrid/TileGrid.cpp
        Sources/Graphics/TileGrid/TileGrid.hpp
        Sources/Graphics/UI/Widget/Button.cpp
        Sources/Graphics/UI/Widget/Button.hpp
        Sources/Graphics/UI/Widget/Container.cpp
        Sources/Graphics/UI/Widget/Container.hpp
        Sources/Graphics/UI/Widget/Entry.cpp
        Sources/Graphics/UI/Widget/Entry.hpp
        Sources/Graphics/UI/Widget/Label.cpp
        Sources/Graphics/UI/Widget/Label.hpp
        Sources/Graphics/UI/Widget/Widget.cpp
        Sources/Graphics/UI/Widget/Widget.hpp
        Sources/Graphics/UI/UIModule/UIModule.cpp
        Sources/Graphics/UI/UIModule/UIModule.hpp
        Sources/Graphics/UI/UIModule/AuthUI.cpp
        Sources/Graphics/UI/UIModule/AuthUI.hpp
        Sources/Graphics/UI/UIModule/GameListUI.cpp
        Sources/Graphics/UI/UIModule/GameListUI.hpp
        Sources/Graphics/UI/UIModule/GameProcessUI.cpp
        Sources/Graphics/UI/UIModule/GameProcessUI.hpp
        Sources/Graphics/UI/UI.cpp
        Sources/Graphics/UI/UI.hpp
        Sources/Graphics/UI/UIModules.hpp
        Sources/Graphics/Chat.cpp
        Sources/Graphics/Chat.hpp
        Sources/Graphics/Sprite.cpp
        Sources/Graphics/Sprite.hpp
        Sources/Graphics/TileGrid.hpp
        Sources/Graphics/Window.hpp
        Sources/Graphics/Window.cpp
        Sources/Client.cpp
        Sources/Client.hpp
        Sources/Network.cpp
        Sources/Network.hpp)

set(EXECUTABLE_NAME "GasProject_Client")
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