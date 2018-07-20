/*
 * shadermanager.cpp
 *
 *  Created on: 2 cze 2017
 *      Author: crm
 */

#include "shadermanager.h"

#include "../debug/log.h"
#include "../io/resource.h"

using namespace Engine::Render;


ShaderManager::ShaderManager()
	{
	//
	}

ShaderManager::~ShaderManager()
	{
	//
	}

ShaderPtr ShaderManager::get(const std::string& path)
	{
	Shader *shader;

	if(named.find(path)!=named.end())
		{
		return named[path];
		}

	LOG_INFO("ShaderManager.get: Wczytywanie \"%s\"", path.c_str());

	// Alokacja struktury
	try
		{
		shader=new Shader();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("ShaderManager.get: Nie udalo sie zajac miejsca na shadera");
		return ShaderPtr(nullptr);
		}

	LOG_DEBUG("ShaderManager.get: Zaalokowano pamiec");

	if(!shader->load(path))
		{
		LOG_ERROR("ShaderManager.get: Nie udalo sie wczytac \"%s\"", path.c_str());
		return ShaderPtr(nullptr);
		}

	LOG_SUCCESS("ShaderManager.get: Wczytano \"%s\"", path.c_str());

	shaders.push_back(shader);
	named[path]=shader;

	return ShaderPtr(shader);
	}

void ShaderManager::free(const Shader* shader, bool erase)
	{
	if(shader->refs()>0)
		{
		LOG_DEBUG("ShaderManager.free: Shader [GLid: %u] ma %d referencji, nie jest kasowany", shader->getProgramID(), shader->refs());
		return;
		}

	if(erase)
		{
		for(auto it=named.begin(); it!=named.end(); ++it)
			{
			if(it->second!=shader)
				continue;

			named.erase(it);

			break;
			}

		for(auto it=shaders.begin(); it!=shaders.end(); ++it)
			{
			if(*it!=shader)
				continue;

			shaders.erase(it);

			break;
			}
		}

	LOG_DEBUG("ShaderManager.free: Kasowanie shadera [GLid: %u]", shader->getProgramID());

	delete shader;
	}
