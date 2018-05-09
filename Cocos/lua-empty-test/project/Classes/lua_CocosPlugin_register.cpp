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

int SayHi(lua_State* L)
{
	const char* pStr = nullptr;
	if (lua_isstring(L, 1))
		pStr = lua_tostring(L, 1);

	char pNewStr[256] = {};
	sprintf(pNewStr, "Hi %s", pStr);

	lua_pushstring(L, pNewStr);
	return 1;
}

int TestCallback(lua_State* L)
{
	const char* pStr = nullptr;
	if (lua_isstring(L, 1))
		pStr = lua_tostring(L, 1);

	int cb = toluafix_ref_function(L, 2, 0);

	char pNewStr[256] = {};
	sprintf(pNewStr, "Hello %s", pStr);

	lua_pushstring(L, pNewStr);

	LuaEngine* engine = LuaEngine::getInstance();
	engine->getLuaStack()->executeFunctionByHandler(cb, 1);

	return 0;
}

void RegisterByLua(lua_State* L)
{
	lua_register(L, "CppAdd", Add);
	lua_register(L, "CppSayHi", SayHi);
	lua_register(L, "CppTestCallback", TestCallback);
}

void RegisterByToLua(lua_State* L)
{
	tolua_open(L);
	tolua_module(L, nullptr, 0);
	tolua_beginmodule(L, nullptr);
	tolua_function(L, "CppAdd", Add);
	tolua_function(L, "CppSayHi", SayHi);
	tolua_function(L, "CppTestCallback", TestCallback);
	tolua_endmodule(L);
}

int register_CocosPlugin_module(lua_State* L)
{
	RegisterByToLua(L);
    return 1;
}