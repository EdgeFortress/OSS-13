# EdgeFortress: Gas Project

This project is an attempt to create engine for multiplayer game about Space Station. Now it's at an early stage, but a lot of basic elements are already realized, including the most important one: network.

In the future this project will be used as a basis for [Space Station 13](https://spacestation13.com/) game remake.

## Compile

The easiest way to compile GasProject now is a Visual Studio 2017 solution. To compile it, you should install dependencies via [vcpkg](https://github.com/Microsoft/vcpkg). Just follow vcpkg readme 
and install SFML with next instructions:

```
vcpkg install sfml:x64-windows-static
vcpkg install sfml:x86-windows-static
```

After that, VS2017 should compile GasProject.sln without any problems.

Also CMakeLists.txt is provided, but it out of date.

## How to Play

In the beginning you need to start the server and then the client. You will see the authorization window.

To log in you can enter the following login and password respectively: admin, password. Also you can create new account, but be careful: the database is not implemented yet, so data storage is not safe.

When logged in select the available game on the newly launched server to start playing.

### Controls

* Move with WASD or arrows
* Press G to become a 'G'host/return to the body
* Press B to 'B'uild a Wall under your character

Also you can choose a chat window with Left Mouse Button or by pressing Tab and send some messages to other players.
