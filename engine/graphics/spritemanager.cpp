/*
 * spritemanager.cpp
 *
 *  Created on: 31 maj 2017
 *      Author: crm
 */

#include "spritemanager.h"

#include <set>

#include "image.h"

#include "../debug/log.h"
#include "../utils/stringparser.h"

using namespace Engine::Graphics;
using Engine::Utils::StringParser;

SpriteManager::SpriteManager()
	{
	//
	}

SpriteManager::~SpriteManager()
	{
	//
	}


bool SpriteManager::isValid(Sprite& spr)
	{
	bool valid=true;

	if(spr.getAnimationCount()<1u)
		{
		LOG_ERROR("SpriteManager.isValid: Sprite musi miec co najmniej jedna animacje");
		valid=false;
		}

	std::set<std::string> points;
	std::set<std::string> layers;

	LOG_DEBUG("SpriteManager.isValid: Sprite: \"%s:%s\"", spr.getPath().c_str(), spr.getName().c_str());

	for(unsigned il=0u; il<spr.getLayerCount(); ++il)
		{
//		LOG_DEBUG("SpriteManager.isValid: - Texture: %s", spr.getLayer(il)->getName().c_str());
		}

	for(unsigned ia=0u; ia<spr.getAnimationCount(); ++ia)
		{
//		LOG_DEBUG("SpriteManager.isValid: - Animation %u: %s", ia, spr.getAnimationNameByIDX(ia).c_str());

		bool atleastone=false;

		for(unsigned id=0u; id<(unsigned)Sprite::DirectionName::COUNT; ++id)
			{
			Sprite::DirectionName dn=(Sprite::DirectionName)id;
//			Sprite::DirectionName dnclosest=spr[ia].getDirectionClosestExisting(dn);
//
//			LOG_DEBUG("SpriteManager.isValid:   | - Direction: %s (%s); FPS: %.2f; Loop: %u",
//					Sprite::getDirectionName(dn).c_str(), Sprite::getDirectionName(dnclosest).c_str(), spr[ia][dn].getFramesPerSecond(), spr[ia][dn].getLoopFrameIndex());

			if(spr[ia][dn].getFrameCount()>0u)
				{
				atleastone=true;
				}

//			for(unsigned i=0u; i<spr[ia][dn].getFrameCount(); ++i)
//				{
//				const Sprite::Frame& f=spr[ia][dn][i];
//
//				LOG_DEBUG("SpriteManager.isValid:   |   | - Frame: %u; [pos %u %u][size %u %u]",
//					i, f.getImage()->getX(), f.getImage()->getY(), f.getImage()->getW(), f.getImage()->getH());
//				}
			}

		if(!atleastone)
			{
			LOG_ERROR("SpriteManager.isValid: Zaden kierunek animacji \"%s\" na ma przynajmniej jednej klatki",
				spr.getAnimationNameByIDX(ia).c_str());

			valid=false;
			}
		}

	return valid;
	}


SpritePtr SpriteManager::get(const std::string& path)
	{
	Sprite *spr;

	if(named.find(path)!=named.end())
		{
		return named[path];
		}

	LOG_INFO("SpriteManager.get: Wczytywanie \"%s\"", path.c_str());

	try
		{
		spr=new Sprite(path);
		}
	catch(std::bad_alloc&)
		{
		LOG_ERROR("SpriteManager.get: Nie udalo sie zajac miejsca na sprite");
		return SpritePtr(nullptr);
		}

	LOG_DEBUG("SpriteManager.get: Zaalokowano pamiec");

	StringParser sp(path, ".");

	if(sp[sp.count()-1u]=="xml")
		{
		if(!getFromXML(*spr, path))
			{
			LOG_ERROR("SpriteManager.get: Nie udalo sie wczytac sprite \"%s\" z pliku XML", path.c_str());
			delete spr;
			return SpritePtr(nullptr);
			}
		}
	else
		{
		LOG_ERROR("SpriteManager.get: Nieobslugiwany format sprite \"%s\"", sp[sp.count()-1u].c_str());
		delete spr;
		return SpritePtr(nullptr);
		}

	if(!isValid(*spr))
		{
		LOG_ERROR("SpriteManager.get: Sprite \"%s\" nie zostal wczytany poprawnie", path.c_str());
		delete spr;
		return SpritePtr(nullptr);
		}

	if(!spr->finalize())
		{
		LOG_ERROR("SpriteManager.get: Sprite \"%s\" nie zostal wczytany poprawnie", path.c_str());
		delete spr;
		return SpritePtr(nullptr);
		}

	named[path]=spr;

	LOG_SUCCESS("SpriteManager.get: Wczytano \"%s:%s\"", spr->getPath().c_str(), spr->getName().c_str());

	return SpritePtr(spr);
	}


void SpriteManager::free(const Sprite* spr, bool erase)
	{
	if(spr->refs()>0)
		{
		LOG_DEBUG("SpriteManager.free: Sprite \"%s:%s\" ma %d referencji, nie jest kasowany", spr->getPath().c_str(), spr->getName().c_str(), spr->refs());
		return;
		}

	if(erase)
		{
		for(auto it=named.begin(); it!=named.end(); ++it)
			{
			if(it->second!=spr)
				continue;

			LOG_DEBUG("SpriteManager.free: Kasowanie nazwanego rysunku \"%s\"", it->first.c_str());

			named.erase(it);

			break;
			}

		for(auto it=sprites.begin(); it!=sprites.end(); ++it)
			{
			if(*it!=spr)
				continue;

			sprites.erase(it);

			break;
			}
		}

	LOG_DEBUG("SpriteManager.free: Kasowanie sprite \"%s:%s\"", spr->getPath().c_str(), spr->getName().c_str());

	delete spr;
	}
