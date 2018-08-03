/*
 * tsingletarget.cpp
 *
 *  Created on: 1 sie 2018
 *      Author: crm
 */

#include "tsingletarget.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"
#include "unit.h"

using namespace Game;

bool TSingleTarget::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_single.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

bool TSingleTarget::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->setFieldOwner(fposition, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	auto taxiDistance=[](const VectorI& a, const VectorI& b)->int
		{
		return std::abs(a.x-b.x)+std::abs(a.y-b.y);
		};

	for(int y=-upgrade-1; y<=upgrade+1; ++y)
		{
		for(int x=-upgrade-1; x<=upgrade+1; ++x)
			{
			if(taxiDistance(fposition, fposition+VectorI(x, y))>upgrade+1)
				{
				continue;
				}

			level->setFieldOwner(fposition+VectorI(x, y), Level::Field::Owner::PLAYER);
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
	return true;
	}


void TSingleTarget::update(float dt)
	{
	using namespace Engine::Math;

	const Vector position=level->getFieldPosition(fposition);


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
			target->damage(DamageType::SINGLE_TARGET, 1.0f);
			}

		cooldown+=1.5f;
		}
	}

void TSingleTarget::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(fposition);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
	}


float TSingleTarget::getRange() const
	{
	const float FIELD_DIAGONAL=sqrt(level->getFieldWidth()*level->getFieldWidth() + level->getFieldHeight()*level->getFieldHeight());

	switch(upgrade)
		{
		case 3:  return FIELD_DIAGONAL*2.0f;
		case 2:  return FIELD_DIAGONAL*1.75f;
		case 1:  return FIELD_DIAGONAL*1.5f;
		default: return FIELD_DIAGONAL*1.0f;
		}
	}

float TSingleTarget::getCooldown() const
	{
	switch(upgrade)
		{
		case 3:  return 1.0f;
		case 2:  return 1.125f;
		case 1:  return 1.25f;
		default: return 1.5f;
		}
	}