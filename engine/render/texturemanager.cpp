/*
 * TextureManager.cpp
 *
 *  Created on: 29 mar 2017
 *      Author: crm
 */

#include "texturemanager.h"

#include <sstream>
#include <SOIL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "render.h"

#include "../debug/log.h"
#include "../graphics/bitmapmanager.h"
#include "../io/resource.h"

using namespace Engine::Render;

TextureManager::TextureManager()
	{
	//
	}

TextureManager::~TextureManager()
	{
	// Singletony kasują się w losowej kolejności
	// TextureManager może zostać usunięty przed ImageManagerem, przez co program będzie się sypał przy zwalnianiu tekstur używanych przez rysunki
	/*LOG_DEBUG("%s: Kasowanie rysunkow", __FUNCTION__);

	for(auto& image: images)
		delete image;
	images.clear();
	named.clear();*/
	}

TexturePtr TextureManager::get(const std::string& path)
	{
	Texture *img;

	if(named.find(path)!=named.end())
		{
		return named[path];
		}

	LOG_INFO("TextureManager.get: Wczytywanie \"%s\"", path.c_str());

	// Alokacja struktury
	try
		{
		img=new Texture(path);
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie zajac miejsca na rysunek");
		return TexturePtr(nullptr);
		}

	LOG_DEBUG("TextureManager.get: Zaalokowano pamiec");

	// Wczytywanie grafiki
	Graphics::BitmapPtr bmp(path);

	if(!bmp)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie wczytac grafiki \"%s\"", path.c_str());
		delete img;
		return TexturePtr(nullptr);
		}

	// Generowanie tekstur
	glGetError(); // Wyzeruj bledy OpenGL
	glGenTextures(1, &img->idtexture);

	GLenum err;
	err=glGetError();

	if(!img->idtexture || err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie dostac tekstury dla \"%s\"", path.c_str());
		LOG_ERROR("TextureManager.get: Blad: %s", gluErrorString(err));
		delete img;
		return TexturePtr(nullptr);
		}

	img->w=bmp->getW();
	img->h=bmp->getH();

	//bmp->dump("out.tga");

	LOG_DEBUG("TextureManager.get: Tekstura wygenerowana [GLid: %u]", img->idtexture);

	GLint ctex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &ctex);
	glBindTexture(GL_TEXTURE_2D, img->idtexture);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp->getPixels());

	//bmp=nullptr;

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie wygenerowac tekstury dla \"%s\"", path.c_str());
		LOG_ERROR("TextureManager.get: Blad: %s", gluErrorString(err));
		glBindTexture(GL_TEXTURE_2D, ctex);
		delete img;
		return TexturePtr(nullptr);
		}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Przy pomniejszaniu - linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// Przy powiekszaniu - nearest - chce PIKSELEEEAAAAA
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// Tylko jesli wspolrzedne sa wieksze od 1 -> profit!... chyba.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// Zobaczymy jak bedzie juz cos widac.

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_WARNING("TextureManager.get: Nie udalo sie ustawic parametrow \"%s\"\n\terr: %04X (%d)", path.c_str(), err, err);

		glBindTexture(GL_TEXTURE_2D, ctex);
		}

	LOG_SUCCESS("TextureManager.get: Wczytano \"%s\" [GLid: %u]", path.c_str(), img->idtexture);

	images.push_back(img);
	named[path]=img;

	glBindTexture(GL_TEXTURE_2D, ctex);

	return TexturePtr(img);
	}

TexturePtr TextureManager::get(const Graphics::Bitmap& bitmap)
	{
	Texture *img;

	LOG_INFO("TextureManager.get: Wczytywanie z surface");

	// Alokacja struktury
	try
		{
		img=new Texture(bitmap.getName());
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("TextureManager.get(surface): Nie udalo sie zajac miejsca na rysunek");
		return TexturePtr(nullptr);
		}

	img->w=bitmap.getW();
	img->h=bitmap.getH();

	// Generowanie tekstur
	glGetError(); // Wyzeruj bledy OpenGL
	glGenTextures(1, &img->idtexture);

	GLenum err;
	err=glGetError();

	if(!img->idtexture || err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get(surface): Nie udalo sie dostac tekstury");
		delete img;
		return TexturePtr(nullptr);
		}

	GLint ctex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &ctex);
	glBindTexture(GL_TEXTURE_2D, img->idtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get(surface): Nie udalo sie wygenerowac tekstury\n\terr: %04X (%d)", err, err);
		glBindTexture(GL_TEXTURE_2D, ctex);
		delete img;
		return TexturePtr(nullptr);
		}

	LOG_DEBUG("TextureManager.get(surface): Wczytano [GLid: %u]", img->idtexture);

	glBindTexture(GL_TEXTURE_2D, ctex);

	images.push_back(img);

	return TexturePtr(img);
	}

TexturePtr TextureManager::get(const Texture& texture, const Engine::Graphics::Bitmap& bitmap)
	{
	Texture *img=nullptr;

	//LOG_INFO("TextureManager.get: Aktualizacja tekstury z bitmapy");

	if(texture.w!=bitmap.getW() || texture.h!=bitmap.getH())
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie zaktualizowac tekstury - rozne wymiary");
		return TexturePtr(nullptr);
		}

	for(auto limg: images)
		{
		if(limg!=&texture)
			continue;

		img=limg;
		break;
		}

	if(!img)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie zaktualizowac tekstury - nie znaleziono na liscie");
		return TexturePtr(nullptr);
		}

	glGetError(); // Wyzeruj bledy OpenGL

	GLint ctex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &ctex);
	glBindTexture(GL_TEXTURE_2D, img->idtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.getPixels());
	//glTexSubImage2D()
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum err;
	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get: Nie udalo sie wygenerowac tekstury\n\terr: %04X (%d)", err, err);
		glBindTexture(GL_TEXTURE_2D, ctex);
		delete img;
		return TexturePtr(nullptr);
		}

	//LOG_DEBUG("TextureManager.get: Zaktualizowano [GLid: %u]", img->idtexture);

	glBindTexture(GL_TEXTURE_2D, ctex);

	return TexturePtr(img);
	}

TexturePtr TextureManager::get(unsigned tex, int w, int h)
	{
	Texture *img;

	LOG_INFO("TextureManager.get: Tworzenie z tekstury");

	// Alokacja struktury
	try
		{
		/*std::stringstream ss;

		ss << "<texture " << tex << " " << w << "," << h << "px>";*/

		img=new Texture();
		}
	catch(std::bad_alloc &)
		{
		LOG_ERROR("TextureManager.get(texture): Nie udalo sie zajac miejsca na rysunek");
		return TexturePtr(nullptr);
		}

	img->idtexture=tex;
	img->w=w;
	img->h=h;

	glGetError(); // Wyzeruj bledy OpenGL

	GLint ctex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &ctex);
	glBindTexture(GL_TEXTURE_2D, img->idtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum err;
	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("TextureManager.get(texture): Nie udalo sie wygenerowac tekstury\n\terr: %04X (%d)", err, err);
		glBindTexture(GL_TEXTURE_2D, ctex);
		delete img;
		return TexturePtr(nullptr);
		}

	LOG_DEBUG("TextureManager.get(texture): Wczytano [GLid: %u]", img->idtexture);

	glBindTexture(GL_TEXTURE_2D, ctex);

	images.push_back(img);

	return TexturePtr(img);
	}


void TextureManager::free(const Texture* img, bool erase)
	{
	if(img->refs()>0)
		{
		LOG_DEBUG("TextureManager.free: Rysunek \"%s\" ma %d referencji, nie jest kasowany", img->getName().c_str(), img->refs());
		return;
		}

	if(erase)
		{
		for(auto it=named.begin(); it!=named.end(); ++it)
			{
			if(it->second!=img)
				continue;

			LOG_DEBUG("TextureManager.free: Kasowanie nazwanego rysunku \"%s\"", it->first.c_str());

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

	Render::getInstance().unbind(img->getTextureID());

	if(img->idtexture)
		glDeleteTextures(1, &img->idtexture);

	LOG_DEBUG("TextureManager.free: Kasowanie \"%s\" [GLid: %u]", img->getName().c_str(), img->idtexture);

	delete img;
	}
