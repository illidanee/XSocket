#include "lua_CocosPlugin_register.h"

int Add(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc != 2)
		return 0;

	int a, b, c;
	if (lua_isnumber(L, 1))
		a = lua_tointeger(L, 1);
	if (lua_isnumber(L, 2))
		b = lua_tointeger(L, 2);
	c = a + b;

	lua_pushinteger(L, c);
	return 1;	
}

int register_CocosPlugin_module(lua_State* L)
{
	lua_register(L, "CppAdd", Add);
    return 1;
}