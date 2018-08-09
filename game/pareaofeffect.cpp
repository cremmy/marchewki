/*
 * pareaofeffect.cpp
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#include "pareaofeffect.h"

#include "level.h"
#include "unit.h"

using namespace Game;

PAreaOfEffect::PAreaOfEffect(const Engine::Math::Vector& target, float damageRange, float damage, float maxSpeed):
	Projectile(DamageType::AOE, damage, maxSpeed), target(target), damageRange(damageRange)
	{
	//
	}

PAreaOfEffect::~PAreaOfEffect()
	{
	//
	}


bool PAreaOfEffect::init()
	{
	sprite=Engine::Graphics::SpritePtr("sprite/projectile.xml");

	if(!sprite)
		{
		return false;
		}

	return true;
	}

void PAreaOfEffect::update(float dt)
	{
	using namespace Engine::Math;

	sprite.update(dt);

	const Vector path=target-position;
	const float pathLength=VectorLength(path);
	direction=path/pathLength;

	if(pathLength<maxSpeed*dt)
		{
		std::vector<Unit*> units;

		if(level->findAllUnitsInRange(target, damageRange, units))
			{
			for(auto unit: units)
				{
				unit->damage(damageType, damage);
				}
			}

		//target->damage(damageType, damage);
		alive=false;
		return;
		}

	position=position+direction*maxSpeed*dt;
	}
