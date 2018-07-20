/*
 * shader.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "../core/refcounted.h"
#include "../math/math.h"
//#include "../math/quaternion.h"

namespace Engine
	{
	namespace Render
		{
		class Shader: public Engine::Core::RefCounted
			{
			private:
				Shader(const Shader& i)=delete;
				Shader& operator=(const Shader& i)=delete;

				GLuint program;
				mutable std::map<std::string, GLint> uniforms;

				bool load(const std::string& path, std::stringstream& ssvert, std::stringstream& ssfrag, bool vert=true, int level=0);

			public:
				Shader(): RefCounted(), program(0u) {}
				~Shader() {clear();}

				bool load(const std::string& path);
				void clear();

				GLint getUniform(const std::string& name) const;
				GLuint getUniformBlock(const std::string& name) const;
				GLuint getProgramID() const {return program;}
			};
		} /* namespace Graphics */
	} /* namespace Engine */
