/*
 * tareaofeffect.cpp
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#include "tareaofeffect.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "consts.h"
#include "level.h"
#include "math_utils.h"
#include "pareaofeffect.h"
#include "unit.h"

using namespace Game;

bool TAreaOfEffect::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_aoe.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	if(!(spriteProjectile=Engine::Graphics::SpritePtr("sprite/projectile.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite pocisku");
		return false;
		}

	cooldown=getCooldown();

	return true;
	}

bool TAreaOfEffect::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->fieldClaim(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	for(int y=-upgrade-1; y<=upgrade+1; ++y)
		{
		for(int x=-upgrade-1; x<=upgrade+1; ++x)
			{
			//if(taxiDistance(fposition, fposition+VectorI(x, y))>upgrade+1)
			//	{
			//	continue;
			//	}

			level->fieldClaim(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}

bool TAreaOfEffect::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	return true;
	}

bool TAreaOfEffect::removeFromLevel()
	{
	using namespace Engine::Math;

	if(target)
		target->unlock();

	for(int y=-upgrade-1; y<=upgrade+1; ++y)
		{
		for(int x=-upgrade-1; x<=upgrade+1; ++x)
			{
			level->fieldRelease(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}


void TAreaOfEffect::update(float dt)
	{
	using namespace Engine::Math;

	const Vector position=level->getFieldPosition(fposition);

	sprite.update(dt);
	spriteUpgrade.update(dt);

	cooldown-=dt;

	if(target && (!target->isAlive() || VectorLength(target->getPosition()-position)>getRange()))
		{
		setTarget(nullptr);
		}

	if(cooldown<=0.0f)
		{
		if(!target)
			{
			setTarget(level->findUnitInRange(position, getRange(), [](Unit* a, Unit* b)->bool {return a->getHP()<b->getHP();}));
			}

		if(target)
			{
			//target->damage(DamageType::SINGLE_TARGET, 1.0f);
			Projectile* projectile=new PAreaOfEffect(
					target->getPosition(),
					TURRET_AREA_OF_EFFECT_EXPLOSION_MOD*getRange(),
					TurretBaseDamage::PLAYER_UNIT_AREA_OF_EFFECT + (TurretBaseDamage::PLAYER_UNIT_AREA_OF_EFFECT-TurretBaseDamage::PLAYER_UNIT_AREA_OF_EFFECT)*(upgrade/(float)MAX_UPGRADE),
					(ProjectileBaseSpeed::AREA_OF_EFFECT + (ProjectileMaxSpeed::AREA_OF_EFFECT-ProjectileBaseSpeed::AREA_OF_EFFECT)*(upgrade/(float)MAX_UPGRADE))*getRange()
					);
			if(!projectile->init())
				{
				cooldown+=10.0f;

				delete projectile;
				LOG_ERROR("Nie udalo sie zainicjowac pocisku");
				return;
				}

			const Vector offset=sprite.getCurrentFrame().getPoint("from");

			projectile->setLevel(level);
			projectile->setPosition(position+Vector(0.0f, 0.0f, offset.y));

			level->addProjectile(projectile);
			}

		cooldown+=getCooldown();
		}
	}


float TAreaOfEffect::getRange() const
	{
	const float FIELD_DIAGONAL=level->getFieldDiagonalSize();//sqrt(level->getFieldWidth()*level->getFieldWidth() + level->getFieldHeight()*level->getFieldHeight());

	switch(upgrade)
		{
		case 3:  return FIELD_DIAGONAL*3.0f;
		case 2:  return FIELD_DIAGONAL*2.25f;
		case 1:  return FIELD_DIAGONAL*1.75f;
		default: return FIELD_DIAGONAL*1.5f;
		}
	}

float TAreaOfEffect::getCooldown() const
	{
	switch(upgrade)
		{
		case 3:  return 1.6f;
		case 2:  return 1.75f;
		case 1:  return 2.0f;
		default: return 2.5f;
		}
	}
