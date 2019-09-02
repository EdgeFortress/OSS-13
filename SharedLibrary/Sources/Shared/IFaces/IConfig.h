#pragma once

#include <string>

#include <Shared/IFaces/IFace.h>
#include <Shared/Geometry/Vec2.hpp>

class IConfig : public IFace {
public:
	virtual std::string GetString(const char *option) const = 0;
	virtual int GetInt(const char *option) const = 0;
	virtual bool GetBool(const char *option) const = 0;
	virtual uf::vec2i GetVec2i(const char *option) const = 0;
	virtual uf::vec2f GetVec2f(const char *option) const = 0;
};
