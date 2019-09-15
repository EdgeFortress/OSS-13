# Open Space Station 13

This project is an attempt to create engine for multiplayer game about Space Station. Now it is at an early stage, but a lot of basic elements are already released, including the most important two: network and script engine.

In the future this project will be used as a basis for [Space Station 13](https://spacestation13.com/) game remake.

* **Windows build:** [![Build status](https://ci.appveyor.com/api/projects/status/mrqydhalvotd0rsl/branch/master?svg=true)](https://ci.appveyor.com/project/Insineer/oss-13-7sux2/branch/master)
* **Linux build:** [![Build Status](https://travis-ci.org/EdgeFortress/OSS-13.svg?branch=master)](https://travis-ci.org/EdgeFortress/OSS-13)

![Discord](https://img.shields.io/discord/577960604204138506)

## Compile

You can build OSS-13 for almost any modern OS, including Windows 7 (and newer), Linux and MacOS. Here you can find build instructions for Windows 10 and Linux (Ubuntu, Debian, Arch, etc.). We don't test compilation on MacOS but it should work too.

The best way to work on OSS-13 is Visual Studio 2017/19 due to unique C++/Python mixed debug mode being very useful when you are coding Game Logic. 
You can use any other development tools but it would be much less convenient due to lack of such debugging tools.

### Windows

Tip: branch 'windows' has latest version checked for building with Visual Studio 2017.

In order to compile OSS-13 on Windows the simplest way is to use Visual Studio 2017 with [VCPKG](https://github.com/Microsoft/vcpkg) packet manager. This instruction should be suitable for Visual Studio 2019 too.

**Step 1.** Install dependencies.

For now the only dependencies you need to install manually are SFML, Python 3 and GTest. The most convenient way to do it is using VCPKG:

1. Install [vcpkg](https://github.com/Microsoft/vcpkg) according their readme. Don't forget to integrate it!
2. Install [SFML](https://www.sfml-dev.org/), Python 3 and [GTest](https://github.com/google/googletest) with following instructions:

```
vcpkg install sfml:x64-windows-static
vcpkg install sfml:x86-windows-static
vcpkg install python3:x64-windows-static
vcpkg install python3:x86-windows-static
vcpkg install gtest:x64-windows-static
vcpkg install gtest:x86-windows-static
```

**Optional:** You can choose desired configuration (x64 or x86; in most cases x64 should be enough) and install only suitable versions. Also you don't need to install GTest if you are not going to run unit tests.

In order to run built server, also you need install Python 3 (x64 or x86) manually via [official site](https://www.python.org/downloads/).
You can add Pythons with different configurations to PATH in solution debug options (OSS13 Server/Properties/Debugging/Environment). Or just add python with one of configurations to system PATH.

**Step 2.** Configure Visual Studio 2017 (not tested with VS2019, but should work too)

Install latest VS2017 with additional options (or update existing installation with VS Installer):

```
Desktop development with C++
    - Just-In-Time debugger
    - VC++ 2017 latest tools (v141) (you should use it with VS2019 too)
    - Windows 10 SDK
Python development
    - Python native development tools (for C++/Python mixed mode debugging)
Individual components
    - GitHub Extension for Visual Studio (optional)
```

**Step 3.** Clone repository.

You can clone this repository as any other git repository with submodules ([pybind](https://github.com/pybind) and [plog](https://github.com/SergiusTheBest/plog)).

Simply use this command from directory where you want to store project:

```
git clone https://github.com/EdgeFortress/OSS-13 --recurse-submodules
```

**Step 4.** Compile and run solution.

* Open OSS13.sln file.
* It is strongly recommended to toggle option "Show All Files" in the header of Solution Explorer window for all C++ projects because we do not configure filters and use folders for cross-platform solution structuring.
* In properties of OSS13 Server project open "Debugging" tab and set "Working Directory" to "$(SolutionDir)". Also set "Debugger" to "Python/Native Debugging" (this debugger is available only when VS module "Python native development tools" is installed).
* Similarly, set "Working Directory" for OSS13 Client project. You don't need to change debugger here, because Python is used only in Server project.
* Choose desired configuration at the top of main VS window (Debug, x64 is most preferable).
* Build Solution (F7). Tests projects can't be compiled if you didn't install GTest in step 1.
* (optional) Run Test projects.
* Run Server and Client.

### Linux

For Linux [CMake](https://cmake.org/) is provided. You should manually install [SFML](https://www.sfml-dev.org/), GTest and Python 3 For Development by your package manager and run cmake as usual.

When cloning don't forget to init submodules:

```
git clone https://github.com/EdgeFortress/OSS-13 --recurse-submodules
```

Before running executables, set Working Directory to repository root, otherwise apps won't be able to pick up resources.

### Tests

Unit Tests are compiled automatically if GTest is installed. You can run manually when it is needed.

## How to Play

In the beginning you need to start the server and then the client. You will see the authorization window.

To log in you can enter the following login and password respectively: "admin", "password". Also you can create new account, but be careful: the database is not implemented yet, so data storage is not safe.

### Controls

* Move with WASD or arrows
* Press G to become a 'G'host / return to the body
* Press Ctrl+D to 'D'rop item.

Also you can choose a chat window with Left Mouse Button or by pressing Tab and send some messages to other players.
