/*
 * bitmap.h
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "../core/refcounted.h"

namespace Engine
	{
	namespace Graphics
		{
		class Bitmap: public Engine::Core::RefCounted
			{
			protected:
				Bitmap(const std::string& name="<unnamed>"): RefCounted(), name(name), surface(nullptr), freepixels(false) {}
				Bitmap(SDL_Surface* img, const std::string& name="<unnamed>"): name(name), surface(img), freepixels(false) {}
				virtual ~Bitmap() {}

				std::string name;
				SDL_Surface* surface;
				bool freepixels;

			public:
				int getW() const {return surface->w;}
				int getH() const {return surface->h;}

				const std::string& getName() const {return name;}
				unsigned* getPixels() {return (unsigned*)surface->pixels;}
				const unsigned* getPixels() const {return (unsigned*)surface->pixels;}
				SDL_Surface* getSurface() const {return surface;}

				unsigned get(int x, int y) const
					{
					if(x<0 || x>=getW() || y<0 || y>=getH())
						return 0u;

					return getPixels()[x+y*getW()];
					}

				void set(int x, int y, unsigned pixel)
					{
					if(x<0 || x>=getW() || y<0 || y>=getH())
						return;

					getPixels()[x+y*getW()]=pixel;
					}

				void dump(const std::string& path);
				void blit(const Bitmap* bmp, int x, int y);

				friend class BitmapManager;
			};

		} /* namespace Graphics */
	} /* namespace Engine */
