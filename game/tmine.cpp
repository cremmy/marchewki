/*
 * tmine.cpp
 *
 *  Created on: 21 sie 2018
 *      Author: crm
 */

#include "tmine.h"

#include "../engine/debug/log.h"

#include "level.h"
#include "psingletarget.h"
#include "unit.h"

using namespace Game;

bool TMine::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_mine.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

bool TMine::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->setFieldOwner(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	return true;
	}

bool TMine::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
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

bool TMine::removeFromLevel()
	{
	return true;
	}


void TMine::update(float dt)
	{
	using namespace Engine::Math;

	const Vector position=level->getFieldPosition(fposition);

	// Odlicz do 0 i czekaj na określoną liczbę ofiar w zasięgu
	if(cooldown>0.0f)
		{
		cooldown-=dt;
		}
	else
		{
		// Boom
		std::vector<Unit*> units;

		if(level->findAllUnitsInRange(position, getRange(), units))
			{
			if(units.size()<=(unsigned)upgrade)
				{
				cooldown=0.1f;
				}
			else
				{
				for(auto unit: units)
					{
					unit->damage(DamageType::AOE, 2.0f-1.5f*upgrade/MAX_UPGRADE);
					}

				cooldown+=getCooldown();
				}
			}
		}
	}


float TMine::getRange() const
	{
	//return level->getFieldDiagonalSize()*0.5f;
	return (level->getFieldHeight()+level->getFieldWidth())*0.25f;
	}

float TMine::getCooldown() const
	{
	switch(upgrade)
		{
		case 4:  return 2.5f;
		case 3:  return 3.5f;
		case 2:  return 4.25f;
		case 1:  return 4.75f;
		default: return 5.0f;
		}
	}
