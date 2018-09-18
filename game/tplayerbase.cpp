/*
 * tplayerbase.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tplayerbase.h"

#include "../engine/debug/log.h"

#include "consts.h"
#include "level.h"
#include "particleemitter.h"

using namespace Game;

bool TPlayerBase::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/base.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	cooldown=getCooldown();

	return true;
	}

bool TPlayerBase::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->fieldClaim(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	const int FIELD_RANGE=(upgrade>=3)?2:1;

	for(int y=-FIELD_RANGE; y<=FIELD_RANGE; ++y)
		{
		for(int x=-FIELD_RANGE; x<=FIELD_RANGE; ++x)
			{
			level->fieldClaim(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
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

	return true;
	}

bool TPlayerBase::removeFromLevel()
	{
	using namespace Engine::Math;

	const int FIELD_RANGE=(upgrade>=3)?2:1;

	for(int y=-FIELD_RANGE; y<=FIELD_RANGE; ++y)
		{
		for(int x=-FIELD_RANGE; x<=FIELD_RANGE; ++x)
			{
			level->fieldRelease(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}


void TPlayerBase::update(float dt)
	{
	using namespace Engine::Math;

	const unsigned FIELDS_ALL=level->getWidth()*level->getHeight();
	const unsigned FIELDS_PLAYER=level->getPlayerFieldCount()-level->getPlayerFarmCount();
	const float PERCENT=(float)FIELDS_PLAYER/FIELDS_ALL;

	sprite.update(dt);
	spriteUpgrade.update(dt);

	if(hp<=0.0f)
		{
		if(level->getPlayerTurretCount()>1)
			{
			cooldown-=dt;

			if(cooldown<=0.0f)
				{
				cooldown=0.5f;

				for(int distance=1; distance<std::max(level->getWidth(), level->getHeight()); ++distance)
					{
					for(int oy=-distance; oy<=distance; ++oy)
						{
						for(int ox=-distance; ox<=distance; ++ox)
							{
							const Level::Field* field=((const Level*)level)->getField(fposition+VectorI(ox, oy));

							if(!field)
								continue;

							const Turret* turret=field->turret;

							if(!turret || turret->getType()==TurretType::PLAYER_BASE || turret->getType()==TurretType::ENEMY_SPAWNER || turret->getType()==TurretType::PLAYER_CARROT_FIELD)
								continue;

							level->destroyTurret(fposition+VectorI(ox, oy), true);
							level->addEmitter(new ParticleEmitter(
								ParticleEmitterType::EXPLOSION,
								level->getFieldPosition(fposition+VectorI(ox, oy)),
								0.5f,
								Engine::Graphics::SpritePtr("sprite/particle_green.xml"),
								200.0f,
								level->getFieldDiagonalSize()*0.5f,
								32.0f, 192.0f, 0.5f));

							return;
							}
						}
					}
				}
			}

		if(upgrade!=0)
			{
			setUpgrade(0);
			sprite.setAnimation("0");
			}
		return;
		}

	if(PERCENT<0.30f)
		{
		if(upgrade!=0)
			{
			setUpgrade(0);
			sprite.setAnimation("0");
			}
		}
	else if(PERCENT<0.55f)
		{
		if(upgrade!=1)
			{
			setUpgrade(1);
			sprite.setAnimation("1");
			}
		}
	else if(PERCENT<0.75f)
		{
		if(upgrade!=2)
			{
			setUpgrade(2);
			sprite.setAnimation("2");
			}
		}
	else if(PERCENT<0.90f)
		{
		if(upgrade!=3)
			{
			setUpgrade(3);
			sprite.setAnimation("3");
			}
		}
	else if(PERCENT<0.95f)
		{
		if(upgrade!=4)
			{
			setUpgrade(4);
			sprite.setAnimation("4");
			}
		}
	else
		{
		if(upgrade!=5)
			{
			setUpgrade(5);
			sprite.setAnimation("5");
			}
		}

	// Jednostki
	if(cooldown>0.0f)
		{
		cooldown-=dt;
		}
	else if(level->getUnlockedCollectiblesCount()>0u)
		{
		cooldown+=getCooldown();

		level->spawnUnit(UnitType::PLAYER_ACOLYTE, fposition, {0, 0}, 1.0f, level->getFieldDiagonalSize()*UNIT_PLAYER_ACOLYTE_SPEED);
		}
	}

float TPlayerBase::getResourceDrain() const
	{
	if(upgrade<1)
		{
		return -level->getResourceDrainFields();
		}

	return getTurretResourceDrain(getType(), upgrade);
	}

float TPlayerBase::getCooldown() const
	{
	return 10.0f-upgrade*1.0f;
	}
