/*
 * tspawner.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tspawner.h"

#include "../engine/debug/log.h"

#include "level.h"

using namespace Game;

bool TSpawner::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_spawner.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	/*if(!(spriteEnemy=Engine::Graphics::SpritePtr("sprite/unit_enemy_infantry.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}*/

	cooldown=10.0f;

	return true;
	}

bool TSpawner::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->setFieldOwner(fposition, Level::Field::Owner::ENEMY))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	level->setFieldOwner(fposition+VectorI(-1,  0), Level::Field::Owner::ENEMY);
	level->setFieldOwner(fposition+VectorI( 1,  0), Level::Field::Owner::ENEMY);
	level->setFieldOwner(fposition+VectorI( 0, -1), Level::Field::Owner::ENEMY);
	level->setFieldOwner(fposition+VectorI( 0,  1), Level::Field::Owner::ENEMY);

	return true;
	}

bool TSpawner::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	return true;
	}

bool TSpawner::removeFromLevel()
	{
	return true;
	}


void TSpawner::update(float dt)
	{
	sprite.update(dt);

	cooldown-=dt;

	if(cooldown<=0.0f)
		{
		if(wave>0)
			{
			--wave;
			cooldown=1.0f;

			level->spawnUnit(UnitType::ENEMY_INFANTRY, fposition, Engine::Math::VectorI(0, 0), 1.0f, level->getFieldDiagonalSize()*0.5f);
			}
		else
			{
			cooldown=5.0f;

			wave=10;
			}
		}
	}

