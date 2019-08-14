#pragma once

#include <string>

#include <Shared/IFaces/IFace.h>

class IConfig : public IFace {
public:
	virtual std::string GetString(const char *option) const = 0;
	virtual int GetInt(const char *option) const = 0;
	virtual bool GetBool(const char *option) const = 0;
};
