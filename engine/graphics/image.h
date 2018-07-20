/*
 * image.h
 *
 *  Created on: 17 kwi 2017
 *      Author: crm
 */

#pragma once

#include "../render/textureptr.h"

namespace Engine
	{
	namespace Graphics
		{
		class ImageManager;

		class Image: public Engine::Core::RefCounted
			{
			protected:
				Render::TexturePtr texture;
				int x, y;
				int w, h;

			public:
				Image(): RefCounted(), texture(nullptr), x(0), y(0), w(0), h(0) {}
				Image(const Render::TexturePtr& texture): RefCounted(), texture(texture), x(0), y(0), w(0), h(0)
					{
					if(!texture)
						return;

					w=texture->getW();
					h=texture->getH();
					}
				Image(const Render::TexturePtr& texture, int x, int y, int w, int h): RefCounted(), texture(texture), x(x), y(y), w(w), h(h)
					{
					if(!texture)
						return;

					w=texture->getW();
					h=texture->getH();
					}
				virtual ~Image() {}

				int getX() const {return x;}
				int getY() const {return y;}
				int getW() const {return w;}
				int getH() const {return h;}
				const Render::TexturePtr& getTexture() const {return texture;}

				friend class ImageManager;
			};
		} /* namespace Graphics */
	} /* namespace Engine */
