#pragma once

#include <Shared/IFaces/INonCopyable.h>

#include <IScriptEngine.h>

class ScriptEngine : public IScriptEngine, public INonCopyable {
public:
	ScriptEngine();
	~ScriptEngine();

	Object *CreateObject(const std::string& module, const std::string& type) final;

	void FillMap(Map *map) final;
	void OnPlayerJoined(Player *player) final;
};
