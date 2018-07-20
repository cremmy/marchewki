/*
 * lua2.cpp
 *
 *  Created on: 02-10-2013
 *      Author: crm
 */

#include "lua.h"

#ifndef LINUX
	#include <Lua/lua.hpp>
	#include <Lua/lualib.h>
	#include <Lua/lauxlib.h>
#else
	#include <lua.hpp>
	#include <lualib.h>
	#include <lauxlib.h>
#endif

#include <sstream>	// tylko do createFunction

#include "../debug/log.h"
#include "../utils/stringparser.h"
//#include "../io/resource.h"

using namespace Engine;
using namespace Engine::Utils;

bool Lua::init()
	{
	clear();

	state=luaL_newstate();
	if(!state)
		{
		LOG_ERROR("Nie udalo sie stworzyc state");

		return false;
		}

	luaL_openlibs(state);

	return true;
	}

void Lua::clear()
	{
	if(state)
		lua_close(state);
	state=nullptr;

	status=0;
	args=0;
	path="<inline script>";
	}


bool Lua::open(const std::string& path)
	{
//	if(!IO::Resource::find(path))
//		{
//		LOG_ERROR("Lua.open: Nie znaleziono pliku \"%s\"", path.c_str());
//
//		return false;
//		}
//
	char *data=nullptr;
//	data=IO::Resource::load(path);

	if(!data)
		{
		LOG_ERROR("Nie udalo sie wczytac pliku \"%s\"", path.c_str());

		return false;
		}

	this->path=path;

	addRegistry("__PATH__", path);

	if(!parse(data))
		{
		LOG_ERROR("Problem przy parsowaniu skryptu \"%s\"", path.c_str());

		delete [] data;

		return false;
		}

	delete [] data;

	return true;
	}

bool Lua::call(const std::string& name)
	{
	if(status!=0)
		return false;

	lua_getglobal(state, name.c_str());
	lua_insert(state, lua_gettop(state)-args);

	status=lua_pcallk(state, args, LUA_MULTRET, 0, 0, nullptr);
	args=0;

	return error();
	}

bool Lua::call(int ref)
	{
	lua_rawgeti(state, LUA_REGISTRYINDEX, ref);

	if(!lua_isfunction(state, -1))
		{
		lua_settop(state, 0);

		return false;
		}

	lua_insert(state, lua_gettop(state)-args);	// Przesun wskaznik na funkcje na spod stosu

	status=lua_pcall(state, args, LUA_MULTRET, 0);
	args=0;

	error();
	return true;
	}

bool Lua::parse(const std::string& script)
	{
	if(status!=0)
		return false;

	/*lua_getglobal(state, "debug");        // push debug-table on the stack
	lua_getfield(state, -1, "traceback");     // push debug.traceback*/
	status=luaL_loadbufferx(state, script.c_str(), script.size(), path.c_str(), nullptr);
	if(!error())
		return false;

	status=lua_pcallk(state, 0, LUA_MULTRET, 0, 0, nullptr);

	return error();
	}


void Lua::push(int val)
	{
	++args;
	lua_pushinteger(state, val);
	}

void Lua::push(float val)
	{
	++args;
	lua_pushnumber(state, val);
	}

void Lua::push(const std::string& str)
	{
	++args;
	lua_pushstring(state, str.c_str());
	}

void Lua::pop()
	{
	lua_settop(state, -2);
	}

void Lua::addGlobal(const std::string& name, int val)
	{
	/*lua_pushstring(state, name.c_str());
	lua_pushinteger(state, val);
	lua_settable(state, LUA_REGISTRYINDEX);*/

	if(!addTableStack(name))
		return;
	lua_pushinteger(state, val);
	lua_settable(state, -3);
	lua_pop(state, 1);
	}

void Lua::addGlobal(const std::string& name, float val)
	{
	if(!addTableStack(name))
		return;
	lua_pushnumber(state, val);
	lua_settable(state, -3);
	lua_pop(state, 1);
	}

void Lua::addGlobal(const std::string& name, const std::string& str)
	{
	if(!addTableStack(name))
		return;
	lua_pushstring(state, str.c_str());
	lua_settable(state, -3);
	lua_pop(state, 1);
	}

void Lua::addGlobal(const std::string& name, void *ptr)
	{
	if(!addTableStack(name))
		return;
	lua_pushlightuserdata(state, ptr);
	lua_settable(state, -3);
	lua_pop(state, 1);
	}

void Lua::addGlobal(const std::string& name, CFunction fptr)
	{
	/*lua_pushcclosure(state, fptr, 0);	// n -> upvalues, 'zewnetrzne' argumenty; n -> liczba rzeczy na stosie pod fptr do popniecia
	lua_setglobal(state, name.c_str());*/

	if(!addTableStack(name))
		return;
	lua_pushcclosure(state, fptr, 0);
	lua_settable(state, -3);
	lua_pop(state, 1);
	}


void Lua::addRegistry(const std::string& name, int val)
	{
	lua_pushstring(state, name.c_str());
	lua_pushinteger(state, val);
	lua_settable(state, LUA_REGISTRYINDEX);
	}

void Lua::addRegistry(const std::string& name, float val)
	{
	lua_pushstring(state, name.c_str());
	lua_pushnumber(state, val);
	lua_settable(state, LUA_REGISTRYINDEX);
	}

void Lua::addRegistry(const std::string& name, const std::string& str)
	{
	lua_pushstring(state, name.c_str());
	lua_pushstring(state, str.c_str());
	lua_settable(state, LUA_REGISTRYINDEX);
	}

void Lua::addRegistry(const std::string& name, void *ptr)
	{
	lua_pushstring(state, name.c_str());
	lua_pushlightuserdata(state, ptr);
	lua_settable(state, LUA_REGISTRYINDEX);
	}


bool Lua::error()
	{
	switch(status)
		{
		case LUA_ERRRUN:
			LOG_ERROR("%s: Problem z wykonaniem: %s", path.c_str(), lua_tostring(state, -1));
		break;

		case LUA_ERRSYNTAX:
			LOG_ERROR("%s: Blad skladni: %s", path.c_str(), lua_tostring(state, -1));
		break;

		case LUA_ERRMEM:
			LOG_ERROR("%s: Problem z alokacja pamieci: %s", path.c_str(), lua_tostring(state, -1));
		break;

		case LUA_ERRGCMM:
			LOG_ERROR("%s: Problem z garbage collectorem: %s", path.c_str(), lua_tostring(state, -1));
		break;

		case LUA_ERRERR:
			LOG_ERROR("%s: Problem obsluga bledu: %s", path.c_str(), lua_tostring(state, -1));
		break;

		case LUA_ERRFILE:
			LOG_ERROR("%s: Problem z wczytaniem pliku: %s", path.c_str(), lua_tostring(state, -1));
		break;

		default:
		case LUA_OK:
			return true;
		break;
		}

	return false;
	}


bool Lua::addTableStack(const std::string& name)
	{
	StringParser pars(name, ".");

	lua_pushglobaltable(state);

	LOG_DEBUG("[%s : %d]", name.c_str(), pars.count());

	for(unsigned i=0u; pars.count()>0 && i<pars.count()-1u; ++i)
		{
		const std::string& t=pars[i];

		lua_pushstring(state, t.c_str());
		lua_gettable(state, -2);

		if(lua_isnil(state, -1))
			{
			//debugStackDump();

			lua_pop(state, 1);

			lua_createtable(state, 0, 0);
			lua_pushstring(state, t.c_str());
			lua_pushvalue(state, -2);
			lua_settable(state, -4);
			lua_remove(state, -2);
			}
		else if(lua_istable(state, -1))
			{
			//debugStackDump();

			lua_remove(state, -2);
			}
		else
			{
			//debugStackDump();

			lua_remove(state, -3);
			return false;
			}
		}

	//lua_pop(state, 1);
	lua_pushstring(state, pars[pars.count()-1].c_str());

	//debugStackDump();

	return true;
	}

bool Lua::functionExists(const std::string& name)
	{
	bool ret;

	lua_getglobal(state, name.c_str());

	ret=lua_isfunction(state, -1);

	lua_settop(state, 0);

	return ret;
	}

void Lua::createFunction(const std::string& name, const std::string& code)
	{
	std::stringstream ss;
	ss << "function " << name << "() " << code << " end";

	parse(ss.str());
	}


std::string Lua::debugGetStackItem(int i)
	{
	int t=lua_type(state, i);

	std::stringstream ss;

	switch(t)
		{
		case LUA_TNUMBER:
			ss << "[Number: " << lua_tonumber(state, i) << "]";
		break;

		case LUA_TSTRING:
			ss << "[String: \"" << lua_tostring(state, i) << "\"]";
		break;

		case LUA_TBOOLEAN:
			ss << "[Boolean: " << (lua_toboolean(state, i)?"true":"false") << "]";
		break;

		case LUA_TTABLE:
			ss << "[Table]";
		break;

		default:
			ss << "[Type: " << lua_typename(state, t) << "]";
		break;
		}

	return ss.str();
	}

void Lua::debugDump()
	{
	//
	}

void Lua::debugStackDump()
	{
	const int size=lua_gettop(state);

	LOG_DEBUG("Lua.debugStackDump:");
	for(int i=1; i<=size; ++i)
		{
		int t=lua_type(state, i);

		switch(t)
			{
			/*case LUA_TTABLE:
				{
				LOG_DEBUG("  [%2d] %s ", i, debugGetStackItem(i).c_str());

				lua_pushnil(state);

				while(lua_next(state, -1))
					{
					lua_settop(state, -1);
					}
				}
			break;*/

			default:
				LOG_DEBUG("  [%2d] %s ", i, debugGetStackItem(i).c_str());
			break;
			}
		}
	}

