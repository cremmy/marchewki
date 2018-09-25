/*
 * tsingletarget.cpp
 *
 *  Created on: 1 sie 2018
 *      Author: crm
 */

#include "tsingletarget.h"

#include "../engine/debug/log.h"

#include "consts.h"
#include "level.h"
#include "math_utils.h"
#include "psingletarget.h"
#include "unit.h"

using namespace Game;

bool TSingleTarget::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_single.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	if(!(spriteProjectile=Engine::Graphics::SpritePtr("sprite/projectile.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite pocisku");
		return false;
		}

	return true;
	}

bool TSingleTarget::updateFieldOwners()
	{
	using namespace Engine::Math;

	if(!fieldClaim(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	for(int y=-upgrade-1; y<=upgrade+1; ++y)
		{
		for(int x=-upgrade-1; x<=upgrade+1; ++x)
			{
			if(MathUtils::taxiDistance(fposition, fposition+VectorI(x, y))>upgrade/2+1)
				{
				continue;
				}

			fieldClaim(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}

bool TSingleTarget::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
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

bool TSingleTarget::removeFromLevel()
	{
	using namespace Engine::Math;

	if(target)
		target->unlock();

	for(int y=-upgrade-1; y<=upgrade+1; ++y)
		{
		for(int x=-upgrade-1; x<=upgrade+1; ++x)
			{
			if(MathUtils::taxiDistance(fposition, fposition+VectorI(x, y))>upgrade/2+1)
				{
				continue;
				}

			fieldRelease(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
			}
		}

	return true;
	}


void TSingleTarget::update(float dt)
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

	if(sprite.isAnimationFinished())
		{
		sprite.setAnimation(0u);
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
			Projectile* projectile=new PSingleTarget(
				target,
				TurretBaseDamage::PLAYER_UNIT_SINGLE_TARGET + (TurretBaseDamage::PLAYER_UNIT_SINGLE_TARGET-TurretBaseDamage::PLAYER_UNIT_SINGLE_TARGET)*(upgrade/(float)MAX_UPGRADE),
				(ProjectileBaseSpeed::SINGLE_TARGET + (ProjectileMaxSpeed::SINGLE_TARGET-ProjectileBaseSpeed::SINGLE_TARGET)*(upgrade/(float)MAX_UPGRADE))*getRange()
				);
			if(!projectile->init())
				{
				cooldown+=10.0f;

				delete projectile;
				LOG_ERROR("Nie udalo sie zainicjowac pocisku");
				return;
				}

			const Vector offset=sprite.getCurrentFrame().getPoint("from");

			projectile->setPosition(position - Vector(0.0f, 0.0f, offset.y));

			level->addProjectile(projectile);

			sprite.setAnimation("shooting");
			}

		cooldown+=getCooldown();
		}
	}


float TSingleTarget::getRange() const
	{
	const float FIELD_DIAGONAL=level->getFieldDiagonalSize();//sqrt(level->getFieldWidth()*level->getFieldWidth() + level->getFieldHeight()*level->getFieldHeight());

	switch(upgrade)
		{
		case 3:  return FIELD_DIAGONAL*2.0f;
		case 2:  return FIELD_DIAGONAL*1.875f;
		case 1:  return FIELD_DIAGONAL*1.75f;
		default: return FIELD_DIAGONAL*1.5f;
		}
	}

float TSingleTarget::getCooldown() const
	{
	switch(upgrade)
		{
		case 3:  return 0.9f;
		case 2:  return 1.05f;
		case 1:  return 1.2f;
		default: return 1.5f;
		}
	}
