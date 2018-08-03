/*
 * turret.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "turret.h"

#include "unit.h"

using namespace Game;

bool Turret::init(Level* level, const Engine::Math::VectorI& fposition)
	{
	this->level=level;
	this->fposition=fposition;

	return true;
	}

void Turret::setTarget(Unit* newTarget)
	{
	if(target)
		{
		target->unlock();
		}

	target=newTarget;

	if(target)
		{
		target->lock();
		}
	}
