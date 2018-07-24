/*
 * level.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "level.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

using namespace Game;

bool Level::init(unsigned w, unsigned h)
	{
	clear();

	if(!(fieldSprite=Engine::Graphics::ImagePtr("image/tile.png")))
		{
		return false;
		}

	return resize(w, h);
	}

void Level::update(float dt)
	{
	//
	}

void Level::print(float tinterp)
	{
	using namespace Engine::Math;

	const unsigned W=getWidth();
	const unsigned H=getHeight();

	for(unsigned y=0u; y<H; ++y)
		{
		for(unsigned x=0u; x<W; ++x)
			{
			Engine::Render::getInstance().draw(Orientation::FLAT_XY+Vector(x*fieldSprite->getW(), (y+1u)*fieldSprite->getH()), fieldSprite);
			}
		}
	}

void Level::clear()
	{
	for(unsigned y=0u; y<field.size(); ++y)
		{
		for(unsigned x=0u; x<field[y].size(); ++x)
			{
			delete field[y][x];
			}

		field[y].clear();
		}

	field.clear();

	fieldSprite=nullptr;
	}


bool Level::resize(unsigned w, unsigned h)
	{
	const unsigned OLD_H=field.size();
	const unsigned OLD_W=field.empty()?0u:field[0u].size();

	// Pomniejszanie, skasuj nadmiarowe pola
	if(h<OLD_H)
		{
		for(unsigned y=h; y<OLD_H; ++y)
			{
			for(unsigned x=0u; x<OLD_W; ++x)
				{
				delete field[y][x];
				}
			field[y].clear();
			}
		}

	field.resize(h);

	if(h>OLD_H)
		{
		for(unsigned y=OLD_H; y<h; ++y)
			{
			field[y].resize(w);

			for(unsigned x=0u; x<OLD_W; ++x)
				{
				field[y][x]=nullptr;
				}
			}
		}

	if(w<OLD_W)
		{
		for(unsigned y=0; y<h; ++y)
			{
			// Tutaj już musi zostać field[y].size(), bo przy powiększaniu w H tworzone są tablice odpowiedniej wielkości
			for(unsigned x=w; x<field[y].size(); ++x)
				{
				delete field[y][x];
				}

			field[y].resize(w);
			}
		}
	else if(w>OLD_W)
		{
		for(unsigned y=0; y<h; ++y)
			{
			for(unsigned x=OLD_W; x<field[y].size(); ++x)
				{
				field[y][x]=nullptr;
				}
			}
		}

	return true;
	}

bool Level::resizeIncreaseXByOne()
	{
	return resize(field.empty()?1u:field[0u].size()+1u, field.size());
	}

bool Level::resizeIncreaseYByOne()
	{
	return resize(field.empty()?1u:field[0u].size(), field.size()+1u);
	}


Level::Field* Level::getField(unsigned x, unsigned y)
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}

const Level::Field* Level::getField(unsigned x, unsigned y) const
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}


Engine::Math::Vector Level::getFieldPosition(unsigned x, unsigned y) const
	{
	return Engine::Math::Vector(x*fieldSprite->getW()+fieldSprite->getW()*0.5, y*fieldSprite->getH()+fieldSprite->getH()*0.5);
	}

bool Level::getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, unsigned& x, unsigned& y)
	{
	//LOG_DEBUG(LOG_STR_VECTOR(position));
	//LOG_DEBUG(LOG_STR_VECTOR(direction));

	using namespace Engine::Math;

	const float Z_DISTANCE=position.z/direction.z;
	const Vector HIT=position-direction*Z_DISTANCE;

	if(HIT.x<0 || HIT.y<0)
		{
		return false;
		}

	const int HIT_X=HIT.x/fieldSprite->getW();
	const int HIT_Y=HIT.y/fieldSprite->getH();

	//LOG_DEBUG("Dist: %f; Hit: %f, %f, %f; %d %d", Z_DISTANCE, HIT.x, HIT.y, HIT.z, HIT_X, HIT_Y);

	if(/*HIT_X<0 ||*/ HIT_X>=(int)getWidth() ||
	   /*HIT_Y<0 ||*/ HIT_Y>=(int)getHeight())
		return false;

	x=HIT_X;
	y=HIT_Y;

	return true;
	}
