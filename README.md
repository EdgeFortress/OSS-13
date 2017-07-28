# EdgeFortress: Gas Project

This project is an attempt to create engine for multiplayer game about Space Station. Now it's at an early stage, but a lot of basic elements are already realized, including the most important one: network.

In the future this project will be used as a basis for [Space Station 13](https://spacestation13.com/) game remake.

## Compile

The easiest way to compile project now is a Visual Studio 2015 solution. Also CMake is provided, but there might be some issues with non-VS2015 compilers.

## How to Play

In the beginning you need to start the server and then the client. You will see the authorization window.

To log in you can enter the following login and password respectively: admin, password. Also you can create new account, but be carefull: the database is not implemented yet, so data storage is not safe.

When loged in select the available game on the newly launched server to start playing.

### Controls

* Move with WASD or arrows
* Press G to become a 'G'host/return to the body

Also you can choose a chat window with Left Mouse Button or by pressing Tab and send some messages to other players.
