/*
 * shaderptr.cpp
 *
 *  Created on: 1 cze 2017
 *      Author: crm
 */

#include "shaderptr.h"

#include "render.h"
#include "shadermanager.h"

using namespace Engine::Render;

ShaderPtr::ShaderPtr(const std::string& path): shader(nullptr)
	{
	assign(ShaderManager::getInstance().get(path));
	}

void ShaderPtr::assign(Shader* nshader)
	{
	if(shader)
		{
		shader->unref();
		if(shader->refs()<1)
			ShaderManager::getInstance().free(shader);
		}

	shader=nshader;

	if(shader)
		shader->ref();
	}

void ShaderPtr::setUniforms() const
	{
	if(!shader)
		return;

	unsigned utidx=1u;

	for(auto pair: uvalues)
		{
		const Uniform &u=pair.second;

		switch(u.type)
			{
			case Uniform::Type::INT:
				glUniform1i(pair.first, u.value.uint);
			break;

			case Uniform::Type::FLOAT:
				glUniform1f(pair.first, u.value.ufloat);
			break;

			case Uniform::Type::VECTOR:
				glUniform4fv(pair.first, 1, u.value.uvec);
			break;

			case Uniform::Type::QUATERNION:
				glUniform4fv(pair.first, 1, u.value.uquat);
			break;

			case Uniform::Type::MATRIX:
				glUniformMatrix4fv(pair.first, 1, true, u.value.umatr); // 3 arg, bool -> transpose
			break;

			case Uniform::Type::TEXTURE:
				Render::getInstance().bind(utidx, u.value.utex);
				glUniform1i(pair.first, utidx);

				++utidx;
			break;

			default:
				// noop
			break;
			}
		}
	}
