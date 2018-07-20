/*
 * shadermanager.h
 *
 *  Created on: 2 cze 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "shaderptr.h"

namespace Engine
	{
	namespace Render
		{
		class ShaderManager
			{
			protected:
				ShaderManager();
				ShaderManager(const ShaderManager& )=delete;
				~ShaderManager();
				ShaderManager& operator=(const ShaderManager& )=delete;

				std::vector<Shader*> shaders;
				std::map<std::string, Shader*> named;

			public:
				static ShaderManager& getInstance()
					{
					static ShaderManager instance;

					return instance;
					}

				ShaderPtr get(const std::string& path);

				void free(const Shader* shader, bool erase=true);
			};
		} /* namespace Render */
	} /* namespace Engine */
