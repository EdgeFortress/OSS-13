# Open Space Station 13

This project is an attempt to create an engine for a multiplayer game about Space Station. Now it is at an early stage, but a lot of basic elements are already released, including the most important two: network and script engine.

In the future, this project will be used as a basis for [Space Station 13](https://spacestation13.com/) game remake.

* **Windows build:** [![Build status](https://ci.appveyor.com/api/projects/status/mrqydhalvotd0rsl/branch/master?svg=true)](https://ci.appveyor.com/project/Insineer/oss-13-7sux2/branch/master)
* **Linux build:** [![Build Status](https://travis-ci.org/EdgeFortress/OSS-13.svg?branch=master)](https://travis-ci.org/EdgeFortress/OSS-13)

[![Discord](https://img.shields.io/discord/577960604204138506)](https://discord.gg/aGKbRAj)

## Compile

You can build OSS-13 for almost any modern OS, including Windows 7 (and newer), Linux, and MacOS. Here you can find build instructions for Windows 10 and Linux (Ubuntu, Debian, Arch, etc.). We don't test compilation on MacOS but it should work too.

The best way to work on OSS-13 is Visual Studio 2019 due to the unique C++/Python mixed debug mode being very useful when you are coding Game Logic. 
You can use any other development tools but it would be much less convenient due to the lack of such debugging tools.

### Windows

Tip: branch 'windows' has the latest version checked for building with Visual Studio 2019.

To compile OSS-13 on Windows the simplest way is to use Visual Studio 2019 with [VCPKG](https://github.com/Microsoft/vcpkg) packet manager.

**Step 1.** Configure Visual Studio 2019

Install latest VS2019 with additional options (or update existing installation with VS Installer):

```
Desktop development with C++
    - Just-In-Time debugger
    - VC++ 2019 latest tools (v142)
    - Windows 10 SDK
Python development
    - Python native development tools (for C++/Python mixed mode debugging)
Individual components
    - GitHub Extension for Visual Studio (optional)
```

**Step 2.** Clone repository.

You can clone this repository as any other git repository with submodules ([pybind](https://github.com/pybind) and [plog](https://github.com/SergiusTheBest/plog)).

Simply use this command from a directory where you want to store the project:

```
git clone https://github.com/EdgeFortress/OSS-13 --recurse-submodules
```

**Step 3.** Install dependencies.

Dependencies that you need now include SFML, Python 3, Lemon, GTest, and CEF. Most of them can be installed using VCPKG, so only CEF should be installed manually.

First, let's install VCPKG and dependencies which we can install with it:

1. Install [vcpkg](https://github.com/Microsoft/vcpkg) according to their readme. Don't forget to integrate it!
2. Install [SFML](https://www.sfml-dev.org/), Python 3, [Lemon](https://lemon.cs.elte.hu/) and [GTest](https://github.com/google/googletest) with following instructions:

```
vcpkg install sfml:x64-windows-static
vcpkg install sfml:x86-windows-static
vcpkg install python3:x64-windows-static
vcpkg install python3:x86-windows-static
vcpkg install liblemon:x64-windows-static
vcpkg install liblemon:x86-windows-static
vcpkg install gtest:x64-windows-static
vcpkg install gtest:x86-windows-static
```

**Optional:** You can choose the desired configuration (x64 or x86; in most cases, x64 should be enough) and install only suitable versions. Also, you don't need to install GTest if you are not going to run unit tests.

To run the built server, also you need to install Python 3 (x64 or x86) manually via [official site](https://www.python.org/downloads/).
You can add Pythons with different configurations to PATH in solution debug options (OSS13 Server/Properties/Debugging/Environment). Or just add python with one of the configurations to system PATH.

The last dependency, that you need is CEF. To install it you need to run the next .bat script from cmd:

```
 EdgeFortress/CI/appveyor/install_cef.bat [download_path] [solution_path]
```

**download_path** - an absolute path where CEF archive should be downloaded (you can delete it when the script ends installation)
**solution_path** - an absolute path to root folder of OSS13 git repository

This script automatically downloads binary distribution of CEF, unzips it, compiles C++ wrapper, and puts all files where they should be. All projects are already set to look for CEF files in folders where the script put them, so if you run it with the correct path, everything should work out-of-box.


**Step 4.** Compile and run solution.

* Open OSS13.sln file.
* It is strongly recommended to toggle the option "Show All Files" in the header of the Solution Explorer window for all C++ projects because we do not configure filters and use folders for cross-platform solution structuring.
* In properties of OSS13 Server project open "Debugging" tab and set "Working Directory" to "$(SolutionDir)". Also set "Debugger" to "Python/Native Debugging" (this debugger is available only when VS module "Python native development tools" is installed).
* Similarly, set "Working Directory" for OSS13 Client project. You don't need to change a debugger here, because Python is used only in the Server project.
* Choose desired configuration at the top of main VS window (Debug, x64 is most preferable).
* Build Solution (F7). Tests projects can't be compiled if you didn't install GTest in step 1.
* (optional) Run Test projects.
* Run Server and Client.

### Linux

For Linux [CMake](https://cmake.org/) is provided. You should manually install [SFML](https://www.sfml-dev.org/), [Lemon](https://lemon.cs.elte.hu/), GTest and Python 3 For Development by your package manager and run CMake as usual.

When cloning don't forget to init submodules:

```
git clone https://github.com/EdgeFortress/OSS-13 --recurse-submodules
```

Before running executables, set Working Directory to the repository root, otherwise, apps won't be able to pick up resources.

### Tests

Unit Tests are compiled automatically if GTest is installed. You can run manually when it is needed.

## How to Play

In the beginning, you need to start the server and then the client. You will see the authorization window.

To log in you can enter the following login and password respectively: "admin", "password". Also, you can create a new account, but be careful: the database is not implemented yet, so data storage is not safe.

### Controls

* Move with WASD or arrows
* Press G to become a 'G'host / return to the body
* Press Ctrl+D to 'D'rop item.

Also, you can choose a chat window with Left Mouse Button or by pressing Tab and send some messages to other players.
