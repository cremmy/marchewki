/*
 * spritemanager_xml.cpp
 *
 *  Created on: 31 maj 2017
 *      Author: crm
 */

#include "spritemanager.h"

#define TIXML_USE_STL
#include <tinyxml.h>

#include "../debug/log.h"
#include "../graphics/imagemanager.h"
#include "../io/resource.h"
#include "../math/math.h"
#include "../render/textureptr.h"
#include "../utils/stringparser.h"

using namespace Engine::Graphics;

const char TAG_SPRITE[]="sprite";
const char TAG_IMAGE[]="image";
const char TAG_ANIMATION[]="animation";
const char TAG_DIRECTION[]="direction";
const char TAG_FRAME[]="frame";
const char TAG_POINT[]="point";

const char ATTR_NAME[]="name";
const char ATTR_SHADER[]="shader";
//const char ATTR_RELATIVE[]="relative";
const char ATTR_FPS[]="fps";
const char ATTR_LOOPTO[]="loopto";
const char ATTR_OFFSET_X[]="offsetx";
const char ATTR_OFFSET_Y[]="offsety";
const char ATTR_X[]="x";
const char ATTR_Y[]="y";
const char ATTR_W[]="w";
const char ATTR_H[]="h";

bool SpriteManager::getFromXML(Sprite& spr, const std::string& path)
	{
	LOG_INFO("SpriteManager.getFromXML: Wczytywanie sprite z pliku \"%s\"", path.c_str());

	TiXmlDocument xml;

	char *data;

	LOG_DEBUG("SpriteManager.getFromXML: Wczytywanie danych...");
	data=Engine::IO::Resource::load(path);

	if(!data)
		{
		LOG_ERROR("SpriteManager.getFromXML: Nie udalo sie wczytac pliku \"%s\"", path.c_str());
		return false;
		}

	LOG_DEBUG("SpriteManager.getFromXML: Parsowanie pliku XML...");
	xml.Parse(data);
	//delete [] data;

	if(xml.Error())
		{
		LOG_ERROR("SpriteManager.getFromXML: Blad w pliku \"%s\"", path.c_str());
		LOG_ERROR("SpriteManager.getFromXML: Linia %d: %s", xml.ErrorRow(), xml.ErrorDesc());
		return false;
		}

	Render::TexturePtr defaultLayer;

	/*******************************/
	/**** Tag image ****************/
	/*******************************/
	auto parseImage=[&spr, &defaultLayer, &path](TiXmlElement* eimage)->bool
		{
		/*if(!eimage->Attribute(ATTR_NAME))
			{
			LOG_ERROR("SpriteManager.getFromXML.parseImage: Nie znaleziono nazwy warstwy");
			return false;
			}*/

		if(!eimage->GetText())
			{
			LOG_ERROR("SpriteManager.getFromXML.parseImage: Nie znaleziono sciezki do rysunku");
			return false;
			}

		Render::TexturePtr tptr;

		if(eimage->GetText()[0]=='/')
			{
			LOG_DEBUG("SpriteManager.getFromXML.parseImage: Sciezka absolutna, \"%s\"", eimage->GetText());
			tptr=Render::TexturePtr(eimage->GetText()+1);
			}
		else
			{
			/*Utils::StringParser sp(path, "/\\");

			sp.remove(sp.count()-1);*/

			const std::string basepath=path.substr(0, path.rfind('/')+1);

			LOG_DEBUG("SpriteManager.getFromXML.parseImage: Sciezka relatywna, \"%s\"", (basepath+eimage->GetText()).c_str());

			tptr=Render::TexturePtr(basepath+eimage->GetText());
			}

		if(!tptr)
			{
			LOG_ERROR("SpriteManager.getFromXML.parseImage: Nie udalo sie wczytac rysunku \"%s\"", eimage->GetText());
			return false;
			}

		if(eimage->Attribute(ATTR_NAME) && strcmp(eimage->Attribute(ATTR_NAME), Sprite::DEFAULT_LAYER)!=0)
			{
			spr.addLayer(eimage->Attribute(ATTR_NAME), tptr);
			LOG_INFO("SpriteManager.getFromXML.parseImage: Wczytano rysunek \"%s\" na warstwe \"%s\"", eimage->GetText(), eimage->Attribute(ATTR_NAME));
			}
		else
			{
			if(defaultLayer)
				{
				LOG_WARNING("SpriteManager.getFromXML.parseImage: Warstwa glowna juz zdefiniowana");
				return true;
				}

			defaultLayer=tptr;
			LOG_INFO("SpriteManager.getFromXML.parseImage: Wczytano rysunek \"%s\" na warstwe glowna", eimage->GetText(), Sprite::DEFAULT_LAYER);
			}

		return true;
		};

	/*******************************/
	/**** Tag point ****************/
	/*******************************/
	auto parsePoint=[](TiXmlElement* epoint, Sprite::Frame& f)->bool
		{
		int x, y;

		if(!epoint->Attribute(ATTR_NAME))
			{
			LOG_ERROR("SpriteManager.getFromXML.parsePoint: Nie znaleziono nazwy punktu");
			return false;
			}

		if(!epoint->Attribute(ATTR_X))
			{
			LOG_ERROR("SpriteManager.getFromXML.parsePoint: Nie znaleziono wspolrzednej x punktu \"%s\"", epoint->Attribute(ATTR_NAME));
			return false;
			}

		if(!epoint->Attribute(ATTR_Y))
			{
			LOG_ERROR("SpriteManager.getFromXML.parsePoint: Nie znaleziono wspolrzednej y punktu \"%s\"", epoint->Attribute(ATTR_NAME));
			return false;
			}

		epoint->Attribute(ATTR_X, &x);
		epoint->Attribute(ATTR_Y, &y);

		LOG_DEBUG("SpriteManager.getFromXML.parsePoint: [%s][pos %d %d]", epoint->Attribute(ATTR_NAME), x, y);

		f.addPoint(epoint->Attribute(ATTR_NAME), Math::Vector(x, y));

		LOG_DEBUG("SpriteManager.getFromXML.parsePoint: [%s][ingame %.2f %.2f]", epoint->Attribute(ATTR_NAME), f.getPoint(epoint->Attribute(ATTR_NAME)).x, f.getPoint(epoint->Attribute(ATTR_NAME)).y);

		return true;
		};

	/*******************************/
	/**** Tag frame ****************/
	/*******************************/
	auto parseFrame=[parsePoint, &defaultLayer](TiXmlElement* eframe, Sprite::Direction& d)->bool
		{
		int x, y, w, h;
		int ox=0;
		int oy=0;

		if(!eframe->Attribute(ATTR_X) || !eframe->Attribute(ATTR_Y) || !eframe->Attribute(ATTR_W) || !eframe->Attribute(ATTR_H))
			{
			LOG_ERROR("SpriteManager.getFromXML.parseFrame: Nie znaleziono polozenia/wymiarow rysunku (%s, %s, %s, %s)", ATTR_X, ATTR_Y, ATTR_W, ATTR_H);
			return false;
			}

		eframe->Attribute(ATTR_X, &x);
		eframe->Attribute(ATTR_Y, &y);
		eframe->Attribute(ATTR_W, &w);
		eframe->Attribute(ATTR_H, &h);

		if(!eframe->Attribute(ATTR_OFFSET_X) || !eframe->Attribute(ATTR_OFFSET_Y))
			{
			LOG_WARNING("SpriteManager.getFromXML.parseFrame: Nie znaleziono offsetu");
			}
		else
			{
			eframe->Attribute(ATTR_OFFSET_X, &ox);
			eframe->Attribute(ATTR_OFFSET_Y, &oy);
			}

		LOG_DEBUG("SpriteManager.getFromXML.parseFrame: [pos %d %d][size %d %d][offset %d %d]", x, y, w, h, ox, oy);

		Sprite::Frame f;

		f.setImage(ImageManager::getInstance().get(defaultLayer, x, y, w, h));
		f.setOffset(ox, oy);

		TiXmlNode* npoint=eframe->FirstChild(TAG_POINT);

		while(npoint)
			{
			if(!parsePoint(npoint->ToElement(), f))
				return false;

			npoint=npoint->NextSibling(TAG_POINT);
			}

		d.addFrame(f);

		return true;
		};

	/*******************************/
	/**** Tag direction ************/
	/*******************************/
	auto parseDirection=[parseFrame](TiXmlElement* edirection, Sprite::Animation& a)->bool
		{
		std::string name;
		double fps;
		int loopto;

		if(!edirection->Attribute(ATTR_NAME))
			{
			LOG_ERROR("SpriteManager.getFromXML.parseDirection: Nie znaleziono nazwy kierunku");
			return false;
			}

		if(!edirection->Attribute(ATTR_FPS))
			{
			LOG_WARNING("SpriteManager.getFromXML.parseDirection: Nie znaleziono liczby klatek na sekunde, ustawianie na 0");

			fps=0.0;
			}

		if(!edirection->Attribute(ATTR_LOOPTO))
			{
			LOG_WARNING("SpriteManager.getFromXML.parseDirection: Nie znaleziono numeru klatki powrotnej, ustawianie na ostatnia");

			loopto=-1;
			}

		name=edirection->Attribute(ATTR_NAME);
		edirection->Attribute(ATTR_FPS, &fps);
		edirection->Attribute(ATTR_LOOPTO, &loopto);

		LOG_DEBUG("SpriteManager.getFromXML.parseDirection: [%s][fps %f][loop %d]", name.c_str(), fps, loopto);

		Sprite::DirectionName dn=Sprite::getDirectionByName(name);

		if(dn==Sprite::DirectionName::NONE)
			{
			LOG_ERROR("SpriteManager.getFromXML.parseDirection: Nieprawidlowa nazwa kierunku: \"%s\"", name.c_str());
			/*for(unsigned i=0u; i<name.length()+1; ++i)
				LOG_ERROR("[%4d][chr %3d '%c']", i, name[i], name[i]);*/
			LOG_ERROR("SpriteManager.getFromXML.parseDirection: Poprawne nazwy kierunkow:");

			for(unsigned i=0u; i<(unsigned)Sprite::DirectionName::COUNT; ++i)
				{
				LOG_ERROR("SpriteManager.getFromXML.parseDirection:  - %s", Sprite::getDirectionName((Sprite::DirectionName)i).c_str());
				}

			return false;
			}

		Sprite::Direction& d=a.getDirection(dn);

		TiXmlNode* nframe=edirection->FirstChild(TAG_FRAME);

		while(nframe)
			{
			if(!parseFrame(nframe->ToElement(), d))
				return false;

			nframe=nframe->NextSibling(TAG_FRAME);
			}

		// LoopFrameIndex nie pozwala ustawic sie na wiekszy niz jest liczba klatek
		d.setFramesPerSecond(fps);
		d.setLoopFrameIndex(loopto);

		return true;
		};

	/*******************************/
	/**** Tag animation ************/
	/*******************************/
	auto parseAnimation=[parseDirection](TiXmlElement* eanimation, Sprite& spr)->bool
		{
		std::string name;

		if(!eanimation->Attribute(ATTR_NAME))
			{
			LOG_ERROR("SpriteManager.getFromXML.parseAnimation: Nie znaleziono nazwy animacji");
			return false;
			}

		name=eanimation->Attribute(ATTR_NAME);

		if(spr.hasAnimation(name))
			{
			LOG_ERROR("SpriteManager.getFromXML.parseAnimation: Animacja \"%s\" juz zdefiniowana");
			return false;
			}

		spr.addAnimation(name);

		Sprite::Animation& a=spr.getAnimation(name);

		TiXmlNode* ndirection=eanimation->FirstChild(TAG_DIRECTION);

		while(ndirection)
			{
			if(!parseDirection(ndirection->ToElement(), a))
				return false;

			ndirection=ndirection->NextSibling(TAG_DIRECTION);
			}

		return true;
		};

	/*******************************/
	/**** Tag sprite ***************/
	/*******************************/
	auto parseSprite=[parseImage, parseAnimation, &path](TiXmlElement* esprite, Sprite& spr)->bool
		{
		if(!esprite->Attribute(ATTR_NAME))
			{
			LOG_WARNING("SpriteManager.getFromXML.parseSprite: Nie znaleziono nazwy sprite");
			}
		else
			{
			spr.setName(esprite->Attribute(ATTR_NAME));
			}

		if(!esprite->Attribute(ATTR_SHADER))
			{
			LOG_WARNING("SpriteManager.getFromXML.parseSprite: Nie znaleziono nazwy shadera");
			}
		else
			{
			//LOG_DEBUG("SpriteManager.getFromXML.parseSprite: Shader \"%s\"", esprite->Attribute(ATTR_SHADER));

			Render::ShaderPtr shptr;

			if(esprite->Attribute(ATTR_SHADER)[0]=='/')
				{
				LOG_DEBUG("SpriteManager.getFromXML.parseSprite: Sciezka absolutna, \"%s\"", esprite->Attribute(ATTR_SHADER));
				shptr=Render::ShaderPtr(esprite->Attribute(ATTR_SHADER)+1);
				}
			else
				{
				const std::string basepath=path.substr(0, path.rfind('/')+1);

				LOG_DEBUG("SpriteManager.getFromXML.parseSprite: Sciezka relatywna, \"%s\"", (basepath+esprite->Attribute(ATTR_SHADER)).c_str());
				shptr=Render::ShaderPtr(basepath+esprite->Attribute(ATTR_SHADER));
				}

			if(!shptr)
				{
				LOG_ERROR("SpriteManager.getFromXML.parseSprite: Nie udalo sie wczytac shadera \"%s\"", esprite->Attribute(ATTR_SHADER));
				return false;
				}

			spr.setShader(shptr);
			}

		TiXmlNode* nimage=esprite->FirstChild(TAG_IMAGE);

		while(nimage)
			{
			if(!parseImage(nimage->ToElement()))
				return false;

			nimage=nimage->NextSibling(TAG_IMAGE);
			}

		TiXmlNode* nanimation=esprite->FirstChild(TAG_ANIMATION);

		while(nanimation)
			{
			if(!parseAnimation(nanimation->ToElement(), spr))
				return false;

			nanimation=nanimation->NextSibling(TAG_ANIMATION);
			}

		return true;
		};

	/*******************************/
	/**** Wczytanie sprite *********/
	/*******************************/
	TiXmlNode* nsprite=xml.FirstChild(TAG_SPRITE);

	if(!nsprite)
		{
		LOG_ERROR("SpriteManager.getFromXML: Nie znaleziono tagu \"%s\" w pliku \"%s\"", TAG_SPRITE, path.c_str());
		return false;
		}

	if(!parseSprite(nsprite->ToElement(), spr))
		{
		delete [] data;
		return false;
		}

	delete [] data;

	LOG_SUCCESS("SpriteManager.getFromXML: Wczytano sprite z pliku \"%s\"", path.c_str());

	return true;
	}






































