#pragma once

#include <string>

#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

class Object;
class Map;
class Player;

class IScriptEngine {
public:
	virtual Object *CreateObjectByKey(const std::string& typeKey) = 0;
	virtual Object *CreateObject(const std::string& module, const std::string& type = "") = 0;
	virtual std::vector<network::protocol::ObjectType *> GetTypesInfo(const std::string &searchString) = 0;

	virtual void FillMap(Map *map) = 0;
	virtual void OnPlayerJoined(Player *player) = 0;

	virtual ~IScriptEngine() = default;
};
