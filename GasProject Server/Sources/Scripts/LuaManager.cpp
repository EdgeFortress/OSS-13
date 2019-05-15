#include "LuaManager.h"

#include "LuaObject.h"

#include <iostream>

#include <Server.hpp>
#include <World/World.hpp>
#include <World/Tile.hpp>
#include <World/Objects/Projectile.hpp>
#include <World/Objects/Turfs/Airlock.hpp>

#include <Shared/OS.hpp>
#include <Shared/Types.hpp>

LuaManager::LuaManager() :
	state()
{
	state.open_libraries(sol::lib::base,sol::lib::math,sol::lib::package,sol::lib::string,sol::lib::table);
	LuaObject::Register(state);
	state.new_usertype<Tile>("_tile",sol::call_constructor,sol::no_constructor,"GetPos",&Tile::GetPos);
	state.new_usertype<rpos>("_rpos",sol::call_constructor,sol::no_constructor,sol::meta_function::subtraction,[](rpos a,rpos b){return a-b;});
	state.new_usertype<apos>("_apos",sol::call_constructor,sol::no_constructor,sol::meta_function::subtraction,[](apos a,apos b){return rpos(a-b);});
	state.new_usertype<Projectile>("_projectile",sol::call_constructor,sol::factories([](Tile *t, rpos v)
	{
		return CurThreadGame->GetWorld()->CreateObject<Projectile>(t, v);
	}));
	/*
	auto lua_files = FindFilesRecursive(L"Lua", L"*.lua");
	for (auto &lua_file_path : lua_files) {
		state.script_file(std::string(lua_file_path.begin(), lua_file_path.end()).c_str());
	}
	*/
	state.script_file("Lua/main.lua");
}
