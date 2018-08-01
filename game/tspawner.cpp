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
		cooldown=2.0f;

		level->spawnUnit(UnitType::ENEMY_INFANTRY, fposition, Engine::Math::VectorI(0, 0), 1.0f, 32.0f);
		}
	}

void TSpawner::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(fposition);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
	}

