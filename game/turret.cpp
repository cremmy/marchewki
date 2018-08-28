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

	if(!(spriteUpgrade=Engine::Graphics::SpritePtr("sprite/upgrade.xml")))
		{
		return false;
		}

	return true;
	}

void Turret::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(fposition);
	const Orientation billboard=cam.getBillboard(pos);

	Render::getInstance().draw(billboard, sprite);

	if(getType()!=TurretType::PLAYER_CARROT_FIELD && getType()!=TurretType::ENEMY_SPAWNER)
		{
		for(int i=0; i<getUpgrade(); ++i)
			{
			Render::getInstance().draw(billboard - level->getFieldWidth()*0.4f*Vector(1, 0, 0) + 8.0f*i*Vector(0, 1, 0), spriteUpgrade);
			Render::getInstance().draw(billboard + level->getFieldWidth()*0.4f*Vector(1, 0, 0) - 8.0f*i*Vector(0, 1, 0), spriteUpgrade);
			}
		}
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
