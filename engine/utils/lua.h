/*
 * lua2.h
 *
 *  Created on: 02-10-2013
 *      Author: crm
 */

#pragma once

#include <string>

struct lua_State;

namespace Engine
	{
	namespace Utils
		{
		class Lua
			{
			public:
				typedef int (*CFunction)(lua_State *);

			private:
				bool error();
				//bool addTableStack(const std::string& name);

				lua_State *state;

				int status;
				int args;

				std::string path;

				std::string debugGetStackItem(int i);
				bool addTableStack(const std::string& name);

			public:
				 Lua(): state(nullptr), status(0), args(0), path("<>") {}
				~Lua() {clear();}

				operator lua_State*() const {return state;}

				bool init();
				void clear();

				bool open(const std::string& path);
				bool call(const std::string& name);
				bool call(int ref);
				bool parse(const std::string& script);

				inline bool operator() (const std::string& name) {return call(name);}
				inline bool operator= (const std::string& line) {return parse(line);}
				inline operator bool () {return isOk();}

				inline bool isOk() {return (status==0);}

				void push(int val);
				void push(float val);
				void push(const std::string& str);
				void pop();

				void addGlobal(const std::string& name, int val);
				void addGlobal(const std::string& name, float val);
				void addGlobal(const std::string& name, const std::string& str);
				void addGlobal(const std::string& name, void *ptr);
				void addGlobal(const std::string& name, CFunction fptr);

				void addRegistry(const std::string& name, int val);
				void addRegistry(const std::string& name, float val);
				void addRegistry(const std::string& name, const std::string& str);
				void addRegistry(const std::string& name, void *ptr);

				bool functionExists(const std::string& name);
				void createFunction(const std::string& name, const std::string& code=std::string());

				void debugDump();
				void debugStackDump();
			};
		}
	}
