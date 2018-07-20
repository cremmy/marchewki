/*
 * bitmapmanager.cpp
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#include "bitmapmanager.h"

#include <sstream>
#include <SOIL.h>
#include <SDL2/SDL.h>

#include "../debug/log.h"
#include "../io/resource.h"

using namespace Engine::Graphics;

BitmapManager::BitmapManager()
	{
	//
	}

BitmapManager::~BitmapManager()
	{
	/*LOG_DEBUG("%s: Kasowanie rysunkow", __FUNCTION__);

	for(auto& image: images)
		delete image;
	images.clear();
	named.clear();*/
	}

BitmapPtr BitmapManager::get(const std::string& path)
	{
	Bitmap *img;
	int w, h;

	if(named.find(path)!=named.end())
		{
		return named[path];
		}

	LOG_INFO("BitmapManager.get: Wczytywanie \"%s\"", path.c_str());

	// Alokacja struktury
	try
		{
		img=new Bitmap(path);
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie zajac miejsca na rysunek");
		return BitmapPtr(nullptr);
		}

	LOG_DEBUG("BitmapManager.get: Zaalokowano pamiec");

	// Wczytywanie grafiki
	char *resGfx=Engine::IO::Resource::load(path);
	if(resGfx==NULL)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie wczytac grafiki \"%s\"", path.c_str());
		delete img;
		return BitmapPtr(nullptr);
		}

	LOG_DEBUG("BitmapManager.get: Wczytano dane obrazka [%.4f kB]", Engine::IO::Resource::getSize()/1024.0f);

	// Dekodowanie
	unsigned char *pixels=SOIL_load_image_from_memory((unsigned char*)resGfx, Engine::IO::Resource::getSize(), &w, &h, nullptr, SOIL_LOAD_RGBA);
	delete [] resGfx;

	if(!pixels)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie zdekompresowac grafiki \"%s\"\n\terr: %s", path.c_str(), SOIL_last_result());
		delete img;
		return BitmapPtr(nullptr);
		}

	LOG_DEBUG("BitmapManager.get: Odkodowano piksele [%.4f kB]", w*h*4/1024.0f);

	// Generowanie surface
	//SDL_CreateRGBSurfaceFrom(pixels, w, h, 32, 0, 0xFF, 0xFF<<8, 0xFF<<16, 0xFF<<24);
	// !! Poniższa funkcja NIE KOPIUJE pikseli - nie wolno ich teraz kasować, oraz należy usunąć ręcznie (SDL_FreeSurface nie kasuje)
	img->surface=SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, 4*w, SDL_PIXELFORMAT_RGBA32);
	img->freepixels=true;

	//delete [] pixels;

	if(!img->surface)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie wygenerowac surface dla \"%s\"", path.c_str());
		delete img;
		return BitmapPtr(nullptr);
		}

	LOG_SUCCESS("BitmapManager.get: Wczytano \"%s\"", path.c_str());

	images.push_back(img);
	named[path]=img;

	return BitmapPtr(img);
	}

BitmapPtr BitmapManager::get(SDL_Surface* surface)
	{
	Bitmap *img;

	LOG_INFO("BitmapManager.get: Benerowanie bitmapy na podstawie surface");

	// Alokacja struktury
	try
		{
		std::stringstream ss;

		ss << "<surface " << surface->w << "," << surface->h << "px>";

		img=new Bitmap(ss.str());
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie zajac miejsca na rysunek");
		return BitmapPtr(nullptr);
		}

	LOG_DEBUG("BitmapManager.get: Zaalokowano pamiec");

	img->surface=surface;

	LOG_SUCCESS("BitmapManager.get: Wygenerowano bitmape z surface");

	images.push_back(img);

	return BitmapPtr(img);
	}

BitmapPtr BitmapManager::get(const char* data, int w, int h, Type type)
	{
	return BitmapPtr(nullptr);
	}

BitmapPtr BitmapManager::get(int w, int h)
	{
	Bitmap *img;

	LOG_INFO("BitmapManager.get: Generowanie bitmapy %d, %dpx", w, h);

	// Alokacja struktury
	try
		{
		std::stringstream ss;

		ss << "<bitmap " << w << "," << h << "px>";

		img=new Bitmap(ss.str());
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie zajac miejsca na rysunek");
		return BitmapPtr(nullptr);
		}

	LOG_DEBUG("BitmapManager.get: Zaalokowano pamiec");

	img->surface=SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);

	if(!img->surface)
		{
		LOG_ERROR("BitmapManager.get: Nie udalo sie wygenerowac surface dla pustej bitmapy");
		delete img;
		return BitmapPtr(nullptr);
		}

	LOG_SUCCESS("BitmapManager.get: Wygenerowano pusta bitmape");

	images.push_back(img);

	return BitmapPtr(img);
	}


void BitmapManager::free(const Bitmap* img, bool erase)
	{
	if(img->refs()>0)
		{
		LOG_DEBUG("BitmapManager.free: Rysunek \"%s\" ma %d referencji, nie jest kasowany", img->getName().c_str(), img->refs());
		return;
		}

	if(erase)
		{
		for(auto it=named.begin(); it!=named.end(); ++it)
			{
			if(it->second!=img)
				continue;

			named.erase(it);

			break;
			}

		for(auto it=images.begin(); it!=images.end(); ++it)
			{
			if(*it!=img)
				continue;

			images.erase(it);

			break;
			}
		}

	if(img->surface)
		{
		// Bitmapy wygenerowane przez podeslanie danych pikseli wymagaja aby skasowac te piksele
		// Bitmapy wygenerowane przez SDLa (np. zwracane przez get(int, int)) nie potzrebuja tego
		// Zadanie polega na wymysleniu jak stwierdzic kiedy kasowac a kiedy nie
		if(img->freepixels)
			delete [] (unsigned char*)img->surface->pixels;

		SDL_FreeSurface(img->surface);
		}

	LOG_DEBUG("BitmapManager.free: Kasowanie bitmapy \"%s\"", img->getName().c_str());

	delete img;
	}
