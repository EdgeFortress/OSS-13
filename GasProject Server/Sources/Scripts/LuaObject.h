#pragma once

#include <World/Objects/Clothing/Clothing.h>

#include <sol.hpp>
class LuaManager;

class LuaObject: public Clothing {
private:
    sol::table instance;
public:
    LuaObject(std::string type);
    LuaObject(sol::table instance);
    std::string GetType();
    
	virtual bool InteractedBy(Object *);
	virtual void InteractWith(Object *);

	static void Register(sol::state &);
};