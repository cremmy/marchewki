/*
 * imagemanager.h
 *
 *  Created on: 17 kwi 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "imageptr.h"

namespace Engine
	{
	namespace Render
		{
		class TexturePtr;
		}

	namespace Graphics
		{
		class BitmapPtr;
		class Image;
		class ImagePtr;

		class ImageManager
			{
			protected:
				ImageManager();
				ImageManager(const ImageManager& )=delete;
				~ImageManager();
				ImageManager& operator=(const ImageManager& )=delete;

				std::vector<Image*> images;
				std::map<std::string, Image*> named;

			public:
				static ImageManager& getInstance()
					{
					static ImageManager instance;
					//if(!instance)
					//	instance=new ImageManager();

					return instance;
					}

				ImagePtr get(const std::string& path);
				ImagePtr get(const BitmapPtr& bitmap);
				ImagePtr get(const BitmapPtr& bitmap, int x, int y, int w, int h, const std::string& name=std::string());
				ImagePtr get(const Render::TexturePtr& texture);
				ImagePtr get(const Render::TexturePtr& texture, int x, int y, int w, int h, const std::string& name=std::string());

				void free(const Image* img, bool erase=true);

				/*void clear();*/
			};
		} /* namespace Graphics */
	} /* namespace Engine */
