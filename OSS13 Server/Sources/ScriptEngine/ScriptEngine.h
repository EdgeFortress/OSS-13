#pragma once

#include <map>

#include <IScriptEngine.h>

#include <Shared/IFaces/INonCopyable.h>

#include "ObjectType.h"

using ObjectTypeId = std::string;

class ScriptEngine : public IScriptEngine, public INonCopyable {
public:
	ScriptEngine();
	~ScriptEngine();

	Object *CreateObject(const std::string& typeKey) final;
	std::vector<network::protocol::ObjectType *> GetTypesInfo(const std::string &searchString) final;

	void FillMap(Map *map) final;
	void OnPlayerJoined(Player *player) final;

	ObjectType &GetObjectType(const std::string& module, const std::string& type);

private:
	Object *CreateObject(ObjectType &type);

private:
	std::map<ObjectTypeId, std::unique_ptr<ObjectType>> objectTypes;
};
