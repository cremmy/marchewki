/*
 * spriteptr.cpp
 *
 *  Created on: 19 kwi 2017
 *      Author: crm
 */

#include "spriteptr.h"

#include "spritemanager.h"
#include "../debug/log.h"

using namespace Engine::Graphics;

SpritePtr::SpritePtr(): shader(), sprite(nullptr), animation(nullptr), direction(nullptr), frame(nullptr), iddirection(Sprite::DirectionName::DEFAULT), fps(0.0), timer(0.0f)
	{
	//
	}

SpritePtr::SpritePtr(const SpritePtr& sptr): shader(), sprite(nullptr), animation(nullptr), direction(nullptr), frame(nullptr), iddirection(Sprite::DirectionName::DEFAULT), fps(0.0), timer(0.0f)
	{
	assign(sptr);
	}

SpritePtr::SpritePtr(const Sprite* spr): shader(), sprite(nullptr), animation(nullptr), direction(nullptr), frame(nullptr), iddirection(Sprite::DirectionName::DEFAULT), fps(0.0), timer(0.0f)
	{
	assign(spr);
	}

SpritePtr::SpritePtr(const std::string& path): shader(), sprite(nullptr), animation(nullptr), direction(nullptr), frame(nullptr), iddirection(Sprite::DirectionName::DEFAULT), fps(0.0), timer(0.0f)
	{
	assign(SpriteManager::getInstance().get(path));
	}


SpritePtr::~SpritePtr()
	{
	assign(nullptr);
	}


void SpritePtr::assign(const Sprite* nsprite)
	{
	if(sprite)
		{
		sprite->unref();
		if(sprite->refs()<1)
			SpriteManager::getInstance().free(sprite);
		}

	sprite=nsprite;

	shader=nullptr;
	animation=nullptr;
	direction=nullptr;
	frame=nullptr;
	iddirection=Sprite::DirectionName::DEFAULT;

	timer=0.0f;

	if(sprite)
		{
		sprite->ref();

		shader=sprite->getShader();
		animation=&sprite->getDefaultAnimation(); // Zakladam, ze sprite ma przynajmniej jedna animacje
		iddirection=animation->getDirectionClosestExisting(iddirection);
		direction=&animation->getDirection(iddirection); // ... oraz kierunek
		frame=&direction->getFrame(0u); // ... oraz jedna klatke

		fps=direction->getFramesPerSecond();
		}
	}


void SpritePtr::update(float dt)
	{
	timer+=dt;

	const float ANIM_TIME=direction->getFrameCount()/fps;

	//LOG_DEBUG("%f at %f t %f fn %u", dt, ANIM_TIME, timer, getFrameNumber());

	if(timer>=ANIM_TIME)
		{
		timer=timer-ANIM_TIME+direction->getLoopFrameIndex()/fps;
		}

	frame=&direction->getFrame(getFrameNumber());
	}

void SpritePtr::setAnimation(const std::string& name)
	{
	const Sprite::Animation* nanimation=&sprite->getAnimation(name);

	if(nanimation==animation)
		return;

	animation=nanimation;
	iddirection=animation->getDirectionClosestExisting(iddirection);
	direction=&animation->getDirection(iddirection);
	frame=&direction->getFrame(0u);
	timer=0.0f;
	}

void SpritePtr::setAnimation(unsigned idx)
	{
	const Sprite::Animation* nanimation=&sprite->getAnimation(idx);

	if(nanimation==animation)
		return;

	animation=nanimation;
	iddirection=animation->getDirectionClosestExisting(iddirection);
	direction=&animation->getDirection(iddirection);
	frame=&direction->getFrame(0u);
	timer=0.0f;
	}

void SpritePtr::setDirection(Sprite::DirectionName niddirection)
	{
	if(niddirection==iddirection)
		return;

	iddirection=niddirection;
	direction=&animation->getDirection(niddirection);
	timer=0.0f;
	}

void SpritePtr::setFrame(unsigned idx)
	{
	if(idx>=direction->getFrameCount())
		idx=direction->getFrameCount()-1u;

	timer=direction->getAnimationTime()*idx/direction->getFrameCount();
	}

