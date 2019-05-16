#include "LuaObject.h"

#include <Server.hpp>
#include <World/World.hpp>
#include <World/Tile.hpp>

LuaObject::LuaObject(std::string type)
{
	instance = CurThreadGame->GetLM()->GetState().registry()["_LOADED"][type](this);
}

LuaObject::LuaObject(sol::table instance): instance(instance){}

bool LuaObject::InteractedBy(Object *o)
{
	if(this->instance["InteractedBy"]) return this->instance["InteractedBy"](this,(LuaObject*)o); return false;
}

void LuaObject::InteractWith(Object *o)
{
	if(this->instance["InteractWith"]) this->instance["InteractWith"](this,(LuaObject*)o);
}

std::string LuaObject::GetType()
{
	return instance["_type"];
}

void LuaObject::Register(sol::state &state)
{
	state.new_usertype<LuaObject>(
		"_object", sol::call_constructor, sol::factories([](sol::table t)
		{
			return CurThreadGame->GetWorld()->CreateObject<LuaObject>(nullptr, t);
		}),
		"name",     &LuaObject::name,
		"instance", &LuaObject::instance,
		"sprite",   &LuaObject::sprite,
		"GetTile",  &LuaObject::GetTile,
		"slot",     &LuaObject::slot,
		"layer",    &LuaObject::layer,
		"movable",  &LuaObject::movable,
		"density",  &LuaObject::density);
}
