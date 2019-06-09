#pragma once

#include <Shared/IFaces/INonCopyable.h>

#include <IScriptEngine.h>

class ScriptEngine : public IScriptEngine, public INonCopyable {
public:
	ScriptEngine();
	~ScriptEngine();

	Object *CreateObject(const std::string& module, const std::string& type) override;
};
