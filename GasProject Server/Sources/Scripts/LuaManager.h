#pragma once

#include <sol.hpp>

class LuaManager {
private:
	sol::state state;
public:
	LuaManager();
	sol::state& GetState() {return state;}
};