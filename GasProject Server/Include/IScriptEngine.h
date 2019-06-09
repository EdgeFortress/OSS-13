#pragma once

#include <string>

class Object;

class IScriptEngine {
public:
	virtual Object *CreateObject(const std::string& module, const std::string& type = "") = 0;

	virtual ~IScriptEngine() = default;
};
