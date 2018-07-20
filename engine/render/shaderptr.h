/*
 * shaderptr.h
 *
 *  Created on: 1 cze 2017
 *      Author: crm
 */

#pragma once

#include "shader.h"
#include "textureptr.h"

namespace Engine
	{
	namespace Render
		{
		class ShaderPtr
			{
			private:
				struct Uniform
					{
					enum class Type
						{
						INT,
						FLOAT,
						VECTOR,
						QUATERNION,
						MATRIX,
						TEXTURE,
						};

					Uniform(): type(Type::INT) {value.uint=0;}
					Uniform(const Uniform& u): type(u.type), value(u.value) {}
					Uniform(int v): type(Type::INT) {value.uint=v;}
					Uniform(float v): type(Type::FLOAT) {value.ufloat=v;}
					Uniform(const Engine::Math::Vector& v): type(Type::VECTOR)
						{
						value.uvec[0]=v.x;
						value.uvec[1]=v.y;
						value.uvec[2]=v.z;
						value.uvec[3]=v.w;
						}
					Uniform(const Engine::Math::Quaternion& v): type(Type::QUATERNION)
						{
						value.uquat[0]=v.r;
						value.uquat[1]=v.i;
						value.uquat[2]=v.j;
						value.uquat[3]=v.k;
						}
					Uniform(const Engine::Math::Matrix& v): type(Type::MATRIX)
						{
						value.umatr[ 0]=v[0][0];
						value.umatr[ 1]=v[0][1];
						value.umatr[ 2]=v[0][2];
						value.umatr[ 3]=v[0][3];
						value.umatr[ 4]=v[1][0];
						value.umatr[ 5]=v[1][1];
						value.umatr[ 6]=v[1][2];
						value.umatr[ 7]=v[1][3];
						value.umatr[ 8]=v[2][0];
						value.umatr[ 9]=v[2][1];
						value.umatr[10]=v[2][2];
						value.umatr[11]=v[2][3];
						value.umatr[12]=v[3][0];
						value.umatr[13]=v[3][1];
						value.umatr[14]=v[3][2];
						value.umatr[15]=v[3][3];
						}
					Uniform(const Engine::Render::TexturePtr& v): type(Type::TEXTURE)
						{
						value.utex=v->getTextureID();
						}

					Type type;

					union
						{
						int uint;
						float ufloat;
						float uvec[4];
						float uquat[4];
						float umatr[16];
						GLuint utex;
						} value;
					};

				Shader* shader;
				std::map<GLuint, Uniform> uvalues;

				void assign(Shader* nshader);

			public:
				ShaderPtr(): shader(nullptr) {}
				ShaderPtr(const ShaderPtr& shptr): shader(nullptr), uvalues(shptr.uvalues) {assign(shptr.shader);}
				ShaderPtr(Shader* shader): shader(nullptr) {assign(shader);}
				ShaderPtr(const std::string& path);
				~ShaderPtr() {assign(nullptr);}

				operator bool() const {return !!shader;}
				operator Shader*() {return shader;}
				operator const Shader*() const {return shader;}

				ShaderPtr& operator=(const ShaderPtr& shptr) {assign(shptr.shader); uvalues=shptr.uvalues; return *this;}
				ShaderPtr& operator=(ShaderPtr&& shptr)
					{
					Shader* tmp=shader;
					shader=shptr.shader;
					shptr.shader=tmp;

					uvalues=shptr.uvalues;

					return *this;
					}

				Shader& operator*() {return *shader;}
				Shader* operator->() {return shader;}
				const Shader& operator*() const {return *shader;}
				const Shader* operator->() const {return shader;}

				void uniform(const std::string& name, int v) {uvalues[shader->getUniform(name)]=Uniform(v);}
				void uniform(const std::string& name, float v) {uvalues[shader->getUniform(name)]=Uniform(v);}
				void uniform(const std::string& name, const Engine::Math::Vector& v) {uvalues[shader->getUniform(name)]=Uniform(v);}
				void uniform(const std::string& name, const Engine::Math::Quaternion& v) {uvalues[shader->getUniform(name)]=Uniform(v);}
				void uniform(const std::string& name, const Engine::Math::Matrix& v) {uvalues[shader->getUniform(name)]=Uniform(v);}
				void uniform(const std::string& name, const Engine::Render::TexturePtr& v) {uvalues[shader->getUniform(name)]=Uniform(v);}

				void setUniforms() const;
			};

		} /* namespace Render */
	} /* namespace Engine */
