/*
 * tspawner.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tspawner.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"

using namespace Game;

bool TSpawner::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_spawner.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	cooldown=2.0f;

	return true;
	}

bool TSpawner::updateFieldOwners() const
	{
	level->setFieldOwner(x, y, Level::Field::Owner::ENEMY);

	if(level->getFieldOwner(x-1, y)!=Level::Field::Owner::PLAYER)
		{
		level->setFieldOwner(x-1, y, Level::Field::Owner::ENEMY);
		}
	if(level->getFieldOwner(x+1, y)!=Level::Field::Owner::PLAYER)
		{
		level->setFieldOwner(x+1, y, Level::Field::Owner::ENEMY);
		}
	if(level->getFieldOwner(x, y-1)!=Level::Field::Owner::PLAYER)
		{
		level->setFieldOwner(x, y-1, Level::Field::Owner::ENEMY);
		}
	if(level->getFieldOwner(x, y+1)!=Level::Field::Owner::PLAYER)
		{
		level->setFieldOwner(x, y+1, Level::Field::Owner::ENEMY);
		}

	return true;
	}

bool TSpawner::attachToLevel(Level* level, int x, int y)
	{
	if(!Turret::init(level, x, y))
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
	cooldown-=dt;

	if(cooldown<=0.0f)
		{
		cooldown=1.0f;

		// TODO Spawn przeciwnika
		}
	}

void TSpawner::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(x, y);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
	}

