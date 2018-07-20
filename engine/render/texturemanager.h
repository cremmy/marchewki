/*
 * imagemanager.h
 *
 *  Created on: 29 mar 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "textureptr.h"

struct SDL_Surface;

namespace Engine
	{
	namespace Graphics
		{
		class Bitmap;
		class BitmapPtr;
		}

	namespace Render
		{
		class TextureManager
			{
			protected:
				TextureManager();
				TextureManager(const TextureManager& )=delete;
				~TextureManager();
				TextureManager& operator=(const TextureManager& )=delete;

				std::vector<Texture*> images;
				std::map<std::string, Texture*> named;

			public:
				static TextureManager& getInstance()
					{
					static TextureManager instance;
					//if(!instance)
					//	instance=new TextureManager();

					return instance;
					}

				TexturePtr get(const std::string& path);
				TexturePtr get(const Graphics::Bitmap& bitmap);
				TexturePtr get(const Texture& texture, const Graphics::Bitmap& bitmap);
				TexturePtr get(unsigned tex, int w, int h);

				void free(const Texture* img, bool erase=true);

				/*void clear();*/
			};
		} /* namespace Graphics */
	} /* namespace Engine */
