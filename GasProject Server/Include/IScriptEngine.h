#pragma once

#include <string>

class Object;
class Player;

class IScriptEngine {
public:
	virtual Object *CreateObject(const std::string& module, const std::string& type = "") = 0;

	virtual void OnPlayerJoined(Player *player) = 0;

	virtual ~IScriptEngine() = default;
};
