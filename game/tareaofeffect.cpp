/*
 * tareaofeffect.cpp
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#include "tareaofeffect.h"


#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"
#include "pareaofeffect.h"
#include "unit.h"

using namespace Game;

bool TAreaOfEffect::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_aoe.xml")))
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

bool TAreaOfEffect::updateFieldOwners() const
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

bool TAreaOfEffect::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	return true;
	}

bool TAreaOfEffect::removeFromLevel()
	{
	return true;
	}


void TAreaOfEffect::update(float dt)
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
			//target->damage(DamageType::SINGLE_TARGET, 1.0f);
			Projectile* projectile=new PAreaOfEffect(target->getPosition(), getRange()*0.5f, 0.75f, getRange());
			if(!projectile->init())
				{
				cooldown+=10.0f;

				delete projectile;
				LOG_ERROR("Nie udalo sie zainicjowac pocisku");
				return;
				}

			const Vector offset=sprite.getCurrentFrame().getPoint("from");

			projectile->setLevel(level);
			projectile->setPosition(position+Vector(0.0f, 0.0f, offset.y));

			level->addProjectile(projectile);
			}

		cooldown+=1.5f;
		}
	}


float TAreaOfEffect::getRange() const
	{
	const float FIELD_DIAGONAL=level->getFieldDiagonalSize();//sqrt(level->getFieldWidth()*level->getFieldWidth() + level->getFieldHeight()*level->getFieldHeight());

	switch(upgrade)
		{
		case 3:  return FIELD_DIAGONAL*3.0f;
		case 2:  return FIELD_DIAGONAL*2.25f;
		case 1:  return FIELD_DIAGONAL*1.75f;
		default: return FIELD_DIAGONAL*1.5f;
		}
	}

float TAreaOfEffect::getCooldown() const
	{
	switch(upgrade)
		{
		case 3:  return 1.6f;
		case 2:  return 1.75f;
		case 1:  return 2.0f;
		default: return 2.5f;
		}
	}
