#pragma once
#include <SFML/Network.hpp>
#include <string>
using std::string;

class login {
	string name;
public:
	login(string name);
};
class pass {
	string name;
public:
	pass(string name);
};
class Network{
public:
	void create_conntection();
	void authorization(login login, pass password);
	void registration(login login, pass password);
};