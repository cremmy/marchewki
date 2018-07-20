/*
 * image.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

#include "../core/refcounted.h"

namespace Engine
	{
	namespace Render
		{
		class Render;
		class TextureManager;

		class Texture: public Engine::Core::RefCounted
			{
			protected:
				Texture(const std::string& name="<unnamed>"): RefCounted(), name(name), w(0), h(0), idtexture(0u) {}
				Texture(int w, int h, unsigned idtexture, const std::string& name="<unnamed>"): name(name), w(w), h(h), idtexture(idtexture) {}
				virtual ~Texture() {}

				std::string name;
				int w, h;
				GLuint idtexture;

			public:
				const std::string& getName() const {return name;}
				int getW() const {return w;}
				int getH() const {return h;}

				unsigned getTextureID() const {return idtexture;}

				bool dump(const std::string& path) const;

				friend class TextureManager;
				friend class Render;
			};

		} /* namespace Graphics */
	} /* namespace Engine */
