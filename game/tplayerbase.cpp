/*
 * tplayerbase.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tplayerbase.h"

#include "../engine/debug/log.h"

#include "level.h"

using namespace Game;

bool TPlayerBase::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/base.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

bool TPlayerBase::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->setFieldOwner(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	const int FIELD_RANGE=(upgrade>=3)?2:1;

	for(int y=-FIELD_RANGE; y<=FIELD_RANGE; ++y)
		{
		for(int x=-FIELD_RANGE; x<=FIELD_RANGE; ++x)
			{
			level->setFieldOwner(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}

bool TPlayerBase::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	// Level się tym zajmuje
	/*if(!updateFieldOwners())
		{
		LOG_ERROR("Nie udalo sie ustawic wlascicieli pol");
		return false;
		}*/

	return true;
	}

bool TPlayerBase::removeFromLevel()
	{
	return true;
	}


void TPlayerBase::update(float dt)
	{
	const unsigned FIELDS_ALL=level->getWidth()*level->getHeight();
	const unsigned FIELDS_PLAYER=level->getPlayerFieldCount();
	const float PERCENT=(float)FIELDS_PLAYER/FIELDS_ALL;

	sprite.update(dt);

	if(PERCENT<0.15f)
		{
		if(upgrade!=0)
			setUpgrade(0);
		}
	else if(PERCENT<0.25f)
		{
		if(upgrade!=1)
			setUpgrade(1);
		}
	else if(PERCENT<0.50f)
		{
		if(upgrade!=2)
			setUpgrade(2);
		}
	else if(PERCENT<0.75f)
		{
		if(upgrade!=3)
			setUpgrade(3);
		}
	else if(PERCENT<0.90f)
		{
		if(upgrade!=4)
			setUpgrade(4);
		}
	else
		{
		if(upgrade!=5)
			setUpgrade(5);
		}
	}

