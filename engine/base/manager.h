/*
 * manager.h
 *
 *  Created on: 26 lis 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <memory>
#include <string>

namespace Engine
	{
	namespace Base
		{
		template <class T>
		concept bool HasLoadMethod()
			{
			return requires(T a)
				{
				{a.load(std::string())} -> bool;
				};
			}

		template <class Resource> requires HasLoadMethod<Resource>()
		class Manager
			{
			protected:
				Manager() {}
				~Manager()
					{
					for(auto resource: resources)
						delete resource.second;
					}

				std::map<std::string, std::shared_ptr<Resource>> resources;

			public:
				static Manager<Resource>& getInstance()
					{
					Manager<Resource> instance;
					return instance;
					}

				std::weak_ptr<Resource> load(const std::string& name)
					{
					if(auto it=resources.find(name); it!=resources.end())
						{
						return it->second;
						}

					std::shared_ptr<Resource> ptr=new Resource();

					if(!ptr->load(name))
						{
						return nullptr;
						}

					resources[name]=ptr;

					return ptr;
					}
			};
		}
	}


