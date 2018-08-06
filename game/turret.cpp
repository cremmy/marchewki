/*
 * turret.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "turret.h"

#include "../engine/render/render.h"

#include "level.h"
#include "unit.h"

using namespace Game;

bool Turret::init(Level* level, const Engine::Math::VectorI& fposition)
	{
	this->level=level;
	this->fposition=fposition;

	return true;
	}

void Turret::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(fposition);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
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
