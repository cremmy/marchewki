/*
 * tplayerbase.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tplayerbase.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"

using namespace Game;

bool TPlayerBase::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/base.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

bool TPlayerBase::updateFieldOwners() const
	{
	if(!level->setFieldOwner(x, y, Level::Field::Owner::PLAYER))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", x, y);
		return false;
		}

	return true;
	}

bool TPlayerBase::attachToLevel(Level* level, int x, int y)
	{
	if(!Turret::init(level, x, y))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	if(!updateFieldOwners())
		{
		LOG_ERROR("Nie udalo sie ustawic wlascicieli pol");
		return false;
		}

	return true;
	}

bool TPlayerBase::removeFromLevel()
	{
	return true;
	}


void TPlayerBase::update(float dt)
	{
	const unsigned FIELDS_ALL=level->getWidth()*level->getHeight();
	const unsigned FIELDS_PLAYER=level->getPlayerFieldCount();
	const float PERCENT=(float)FIELDS_PLAYER/FIELDS_ALL;

	if(PERCENT<0.10f)
		{
		setUpgrade(0);
		}
	else if(PERCENT<0.25f)
		{
		setUpgrade(1);
		}
	else if(PERCENT<0.50f)
		{
		setUpgrade(2);
		}
	else if(PERCENT<0.75f)
		{
		setUpgrade(3);
		}
	else if(PERCENT<0.90f)
		{
		setUpgrade(4);
		}
	else
		{
		setUpgrade(5);
		}
	}

void TPlayerBase::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector pos=level->getFieldPosition(x, y);

	Render::getInstance().draw(cam.getBillboard(pos), sprite);
	}
