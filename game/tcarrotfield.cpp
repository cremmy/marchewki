/*
 * tcarrotfield.cpp
 *
 *  Created on: 25 sie 2018
 *      Author: crm
 */

#include "tcarrotfield.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"
#include "psingletarget.h"
#include "unit.h"

using namespace Game;

bool TCarrotField::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_farm.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite wiezy");
		return false;
		}

	cooldown=getCooldown();

	return true;
	}

bool TCarrotField::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->fieldClaim(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	return true;
	}

bool TCarrotField::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	cooldown=getCooldown();

	return true;
	}

bool TCarrotField::removeFromLevel()
	{
	if(target)
		target->unlock(); // Tiaa

	level->fieldRelease(fposition, Level::Field::Owner::PLAYER);

	return true;
	}


void TCarrotField::update(float dt)
	{
	using namespace Engine::Math;

	//const Vector position=level->getFieldPosition(fposition);

	sprite.update(dt);

	// Normalne działanie, brak wrogów polu
	if(!panic)
		{
		cooldown-=dt;

		if(cooldown<=0)
			{
			cooldown=getCooldown();

			setUpgrade(upgrade+1);
			sprite.setAnimation(upgrade);
			}

		if(level->isUnitOnField(fposition))
			{
			panic=true;
			cooldown=getCooldown();
			}
		}
	// Wrogie jednostki na polu
	else
		{
		int countPlayer; // getUnitsOnFieldCount zeruje... a przynajmniej powinno
		int countEnemy;  // jw.

		level->getUnitsOnFieldCount(fposition, countEnemy, countPlayer);

		cooldown-=dt*countEnemy + dt*countPlayer*0.25f;

		if(countEnemy+countPlayer<=0)
			{
			panic=false;
			cooldown=getCooldown();
			return;
			}
		else
			{
			if(cooldown<=0)
				{
				cooldown=getCooldown();

				setUpgrade(upgrade-1);
				sprite.setAnimation(upgrade);
				}
			}
		}
	}

float TCarrotField::getCooldown() const
	{
	if(panic)
		return 1.0f;

	switch(upgrade)
		{
		case 6:  return 31.0f;
		case 5:  return 25.0f;
		case 4:  return 20.0f;
		case 3:  return 16.0f;
		case 2:  return 13.0f;
		case 1:  return 11.0f;
		default: return 10.0f;
		}
	}
