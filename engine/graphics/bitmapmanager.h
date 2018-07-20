/*
 * bitmapmanager.h
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "bitmapptr.h"

struct SDL_Surface;

namespace Engine
	{
	namespace Graphics
		{
		class BitmapManager
			{
			public:
				enum class Type
					{
					IMG_32_RGBA,	// Domyslne
					IMG_32_ABGR,	// Wsteczne RGBA
					IMG_32_BGRA,	// Windowsowe 32 bity?						// A jednak nie.
					IMG_32_ARGB,	// Gdzies toto w ogole jest uzywane? o;		// A jednak tak.
					IMG_24_RGB,		// Chyba najprostsza sytuacja o;
					IMG_24_BGR,		// No, jeszcze kolejnosc wsteczna
					IMG_16_RGB_555,	// Nie zrobione~							// Juz zrobione~
					IMG_16_RGB_655,	// Windowsowe 16 bitow?
					IMG_16_RGB_565,	// Nie zrobione~							// Juz zrobione~
					IMG_16_RGB_556,	// Nie zrobione~							// Juz zrobione~
					IMG_8,			// Nope, to nie jest paletowe - skala odcieni szarosci

					IMG_32_WINDOWS=	IMG_32_ARGB,
					IMG_24_WINDOWS=	IMG_24_RGB,
					IMG_16_WINDOWS=	IMG_16_RGB_565,
					IMG_8_WINDOWS=	IMG_8,
					};

			protected:
				BitmapManager();
				BitmapManager(const BitmapManager& )=delete;
				~BitmapManager();
				BitmapManager& operator=(const BitmapManager& )=delete;

				std::vector<Bitmap*> images;
				std::map<std::string, Bitmap*> named;

			public:
				static BitmapManager& getInstance()
					{
					static BitmapManager instance;
					//if(!instance)
					//	instance=new BitmapManager();

					return instance;
					}

				BitmapPtr get(const std::string& path);
				BitmapPtr get(SDL_Surface* surface);
				BitmapPtr get(const char* data, int w, int h, Type type=Type::IMG_32_RGBA);
				BitmapPtr get(int w, int h);

				void free(const Bitmap* img, bool erase=true);

				/*void clear();*/
			};

		} /* namespace Graphics */
	} /* namespace Engine */
