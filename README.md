# Open Space Station 13

This project is an attempt to create engine for multiplayer game about Space Station. Now it's at an early stage, but a lot of basic elements are already realized, including the most important one: network.

In the future this project will be used as a basis for [Space Station 13](https://spacestation13.com/) game remake.

## Compile

### Windows

Tip: branch 'windows' has latest version checked for compiling with Visual Studio 2017.

1.  For now, only that you need  to compile solution is SFML and you can install it via [vcpkg](https://github.com/Microsoft/vcpkg).

1.1. Install [vcpkg](https://github.com/Microsoft/vcpkg) according their readme. Don't forget to integrate it!

1.2. Install [SFML](https://www.sfml-dev.org/) with next instructions:

```
vcpkg install sfml:x64-windows-static
vcpkg install sfml:x86-windows-static
vcpkg install gtest:x64-windows-static
vcpkg install gtest:x86-windows-static
```

2. Open GasProject.sln and Build it, all dependencies will pick up automatically.

### Linux

For Linux [CMake](https://cmake.org/) is provided. You should manually install [SFML](https://www.sfml-dev.org/) by your distributive's package manager and run cmake as usual.

Note: Run executables with Working Directory at the root of repository, otherwise apps will not be able to pick up resources.

### Tests

Unit Tests is available only for cmake now. To compile them, you should also install [GTest](https://github.com/google/googletest).

## How to Play

In the beginning you need to start the server and then the client. You will see the authorization window.

To log in you can enter the following login and password respectively: admin, password. Also you can create new account, but be careful: the database is not implemented yet, so data storage is not safe.

When logged in select the available game on the newly launched server to start playing.

### Controls

* Move with WASD or arrows
* Press G to become a 'G'host/return to the body
* Press B to 'B'uild a Wall under your character
* Press Ctrl+D to 'D'rop item.

Also you can choose a chat window with Left Mouse Button or by pressing Tab and send some messages to other players.
