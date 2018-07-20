/*
 * imagemanager.cpp
 *
 *  Created on: 17 kwi 2017
 *      Author: crm
 */

#include "imagemanager.h"

#include "bitmapptr.h"

#include "../debug/log.h"
#include "../render/texturemanager.h"

using namespace Engine::Graphics;

ImageManager::ImageManager()
	{
	//
	}

ImageManager::~ImageManager()
	{
	/*LOG_DEBUG("%s: Kasowanie rysunkow", __FUNCTION__);

	for(auto& image: images)
		delete image;
	images.clear();
	named.clear();*/
	}


ImagePtr ImageManager::get(const std::string& path)
	{
	Image *img;

	if(named.find(path)!=named.end())
		{
		return named[path];
		}

	LOG_INFO("ImageManager.get: Wczytywanie \"%s\"", path.c_str());

	// Alokacja struktury
	try
		{
		img=new Image();
		}
	catch(std::bad_alloc&)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie zajac miejsca na rysunek");
		return ImagePtr(nullptr);
		}

	LOG_DEBUG("ImageManager.get: Zaalokowano pamiec");

	// Wczytywanie grafiki
	Render::TexturePtr tex=Render::TextureManager::getInstance().get(path);

	if(!tex)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie wczytac tekstury \"%s\"", path.c_str());
		delete img;
		return ImagePtr(nullptr);
		}

	img->x=0;
	img->y=0;
	img->w=tex->getW();
	img->h=tex->getH();
	img->texture=tex;

	LOG_SUCCESS("ImageManager.get: Wczytano \"%s\"", path.c_str());

	images.push_back(img);
	named[path]=img;

	return ImagePtr(img);
	}


ImagePtr ImageManager::get(const BitmapPtr& bitmap)
	{
	Image *img;

	LOG_INFO("ImageManager.get: Wczytywanie rysunku z bitmapy \"%s\"", bitmap->getName().c_str());

	// Alokacja struktury
	try
		{
		img=new Image();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie zajac miejsca na rysunek");
		return ImagePtr(nullptr);
		}

	LOG_DEBUG("ImageManager.get: Zaalokowano pamiec");

	// Wczytywanie grafiki
	Render::TexturePtr texture=Render::TextureManager::getInstance().get(*bitmap);

	if(!texture)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie wczytac tekstury \"%s\"", bitmap->getName().c_str());
		delete img;
		return ImagePtr(nullptr);
		}

	img->x=0;
	img->y=0;
	img->w=texture->getW();
	img->h=texture->getH();
	img->texture=texture;

	LOG_SUCCESS("ImageManager.get: Wczytano \"%s\"", bitmap->getName().c_str());

	images.push_back(img);

	return ImagePtr(img);
	}

ImagePtr ImageManager::get(const BitmapPtr& bitmap, int x, int y, int w, int h, const std::string& name)
	{
	Image *img;

	if(!name.empty() && named.find(name)!=named.end())
		{
		LOG_DEBUG("ImageManager.get: Rysunek \"%s\" z bitmapy \"%s\" juz zostal wczytany %s", name.c_str(), bitmap->getName().c_str(),
			(named[name]->getTexture()->getW()>w || named[name]->getTexture()->getH()>h)?"i jest czescia atlasu":", ale nie jest czescia atlasu");

		return named[name];
		}

	LOG_INFO("ImageManager.get: Wczytywanie rysunku \"%s\" z bitmapy \"%s\"", name.c_str(), bitmap->getName().c_str());

	// Alokacja struktury
	try
		{
		img=new Image();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie zajac miejsca na rysunek");
		return ImagePtr(nullptr);
		}

	LOG_DEBUG("ImageManager.get: Zaalokowano pamiec");

	// Wczytywanie grafiki
	Render::TexturePtr texture=Render::TextureManager::getInstance().get(*bitmap);

	if(!texture)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie wczytac tekstury \"%s\"", name.c_str());
		delete img;
		return ImagePtr(nullptr);
		}

	img->x=x;
	img->y=y;
	img->w=w;
	img->h=h;
	img->texture=texture;

	if(!name.empty())
		{
		LOG_SUCCESS("ImageManager.get: Wczytano rysunek \"%s\" z bitmapy \"%s\"", name.c_str(), texture->getName().c_str());
		}
	else
		{
		LOG_SUCCESS("ImageManager.get: Wczytano rysunek z bitmapy \"%s\"", texture->getName().c_str());
		}

	images.push_back(img);

	if(!name.empty())
		named[name]=img;

	return ImagePtr(img);
	}


ImagePtr ImageManager::get(const Render::TexturePtr& texture)
	{
	Image *img;

	LOG_INFO("ImageManager.get: Wczytywanie rysunku z tekstury \"%s\"", texture->getName().c_str());

	// Alokacja struktury
	try
		{
		img=new Image();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie zajac miejsca na rysunek");
		return ImagePtr(nullptr);
		}

	LOG_DEBUG("ImageManager.get: Zaalokowano pamiec");

	img->x=0;
	img->y=0;
	img->w=texture->getW();
	img->h=texture->getH();
	img->texture=texture;

	LOG_SUCCESS("ImageManager.get: Wczytano \"%s\"", texture->getName().c_str());

	images.push_back(img);

	return ImagePtr(img);
	}

ImagePtr ImageManager::get(const Render::TexturePtr& texture, int x, int y, int w, int h, const std::string& name)
	{
	Image *img;

	if(!name.empty() && named.find(name)!=named.end())
		{
		LOG_DEBUG("ImageManager.get: Rysunek \"%s\" z tekstury \"%s\" juz zostal wczytany %s", name.c_str(), texture->getName().c_str(),
			(named[name]->getTexture()->getW()>w || named[name]->getTexture()->getH()>h)?"i jest czescia atlasu":", ale nie jest czescia atlasu");

		return named[name];
		}

	if(!name.empty())
		LOG_INFO("ImageManager.get: Wczytywanie rysunku \"%s\" z tekstury \"%s\"", name.c_str(), texture->getName().c_str());
	else
		LOG_INFO("ImageManager.get: Wczytywanie rysunku z tekstury \"%s\"", texture->getName().c_str());

	// Alokacja struktury
	try
		{
		img=new Image();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("ImageManager.get: Nie udalo sie zajac miejsca na rysunek");
		return ImagePtr(nullptr);
		}

	LOG_DEBUG("ImageManager.get: Zaalokowano pamiec");

	img->x=x;
	img->y=y;
	img->w=w;
	img->h=h;
	img->texture=texture;

	if(!name.empty())
		{
		LOG_SUCCESS("ImageManager.get: Wczytano rysunek \"%s\" z tekstury \"%s\"", name.c_str(), texture->getName().c_str());
		}
	else
		{
		LOG_SUCCESS("ImageManager.get: Wczytano rysunek z tekstury \"%s\"", texture->getName().c_str());
		}

	images.push_back(img);

	if(!name.empty())
		named[name]=img;

	return ImagePtr(img);
	}


void ImageManager::free(const Image* img, bool erase)
	{
	if(img->refs()>0)
		{
		LOG_DEBUG("ImageManager.free: Rysunek z tekstury \"%s\" ma %d referencji, nie jest kasowany", img->texture->getName().c_str(), img->refs());
		return;
		}

	if(erase)
		{
		for(auto it=named.begin(); it!=named.end(); ++it)
			{
			if(it->second!=img)
				continue;

			LOG_DEBUG("ImageManager.free: Kasowanie nazwanego rysunku \"%s\"", it->first.c_str());

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

	LOG_DEBUG("ImageManager.free: Kasowanie rysunku z tekstury \"%s\"", img->texture->getName().c_str());

	delete img;
	}


/*void ImageManager::clear()
	{
	LOG_INFO("ImageManager.clear: Kasowanie wszystkich rysunkow");
	LOG_DEBUG("ImageManager.clear: [size: %u][named: %u]", images.size(), named.size());

	for(auto img: images)
		free(img);

	LOG_INFO("ImageManager.clear: Wykasowano rysunki");
	}*/

