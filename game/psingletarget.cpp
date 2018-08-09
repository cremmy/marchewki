/*
 * plinear.cpp
 *
 *  Created on: 4 sie 2018
 *      Author: crm
 */

#include "psingletarget.h"

#include "unit.h"

using namespace Game;

PSingleTarget::PSingleTarget(Unit* target, float damage, float maxSpeed): Projectile(DamageType::SINGLE_TARGET, damage, maxSpeed)
	{
	this->target=target;
	this->target->lock();
	}

PSingleTarget::~PSingleTarget()
	{
	if(target)
		{
		target->unlock();
		}
	}


bool PSingleTarget::init()
	{
	sprite=Engine::Graphics::SpritePtr("sprite/projectile.xml");

	if(!sprite)
		{
		return false;
		}

	return true;
	}

void PSingleTarget::update(float dt)
	{
	using namespace Engine::Math;

	sprite.update(dt);

	if(!target)
		{
		maxSpeed-=maxSpeed*dt;

		if(maxSpeed*dt<1.0f || position.z<0.0f)
			{
			alive=false;
			}

		position=position+direction*maxSpeed*dt;
		}
	else
		{
		if(!target->isAlive())
			{
			target->unlock();
			target=nullptr;
			return;
			}

		const Vector path=target->getPosition()-position;
		const float pathLength=VectorLength(path);
		direction=path/pathLength;

		if(pathLength<maxSpeed*dt)
			{
			target->damage(damageType, damage);
			alive=false;
			return;
			}

		position=position+direction*maxSpeed*dt;
		}
	}
