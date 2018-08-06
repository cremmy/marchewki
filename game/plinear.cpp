/*
 * plinear.cpp
 *
 *  Created on: 4 sie 2018
 *      Author: crm
 */

#include "plinear.h"

#include "unit.h"

using namespace Game;

PLinear::PLinear(Unit* target, DamageType damageType, float damage, float maxSpeed): Projectile(damageType, damage, maxSpeed)
	{
	this->target=target;
	this->target->lock();
	}

PLinear::~PLinear()
	{
	if(target)
		{
		target->unlock();
		}
	}


bool PLinear::init()
	{
	sprite=Engine::Graphics::SpritePtr("sprite/projectile.xml");

	if(!sprite)
		{
		return false;
		}

	return true;
	}

void PLinear::update(float dt)
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
