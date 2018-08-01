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
	// noop
	}

void TSingleTarget::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(fposition);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
	}
