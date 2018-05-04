#ifndef __LUA_COCOSPLUGIN_REGISTER_H__
#define __LUA_COCOSPLUGIN_REGISTER_H__

#include "cocos2d.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

int Add(lua_State* L);

int register_CocosPlugin_module(lua_State* L);

#endif  // __LUA_COCOSPLUGIN_REGISTER_H__

