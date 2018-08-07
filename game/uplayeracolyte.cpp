/*
 * uplayeracolyte.cpp
 *
 *  Created on: 7 sie 2018
 *      Author: crm
 */

#include "uplayeracolyte.h"

#include "../engine/debug/log.h"

#include "collectible.h"
#include "level.h"
#include "turret.h"

using namespace Game;

bool UPlayerAcolyte::init(Level* level, const Engine::Math::VectorI& fposition, const Engine::Math::VectorI& /*target*/)
	{
	this->level=level;
	this->position=level->getFieldPosition(fposition);

	targetCollectible=level->findUnlockedCollectible();
	target=targetCollectible->getPosition();

	if(!(sprite=Engine::Graphics::SpritePtr("sprite/unit_player_acolyte.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	level->findPath(path, targetCollectible->getPosition());
	if(path.empty())
		{
		LOG_WARNING("Nie udalo sie znalezc sciezki do zbierajki");
		}

	return true;
	}

void UPlayerAcolyte::update(float dt)
	{
	using namespace Engine::Math;

	sprite.update(dt);

	if(!isAlive())
		{
		return;
		}

	if(collectCooldown>0.0f)
		{
		collectCooldown-=dt;
		return;
		}

	const float DISTANCE=speed*dt;

	if(targetCollectible)
		{
		/*if(pathSearchCooldown>0.0f)
			{
			pathSearchCooldown-=dt;
			}
		else if(path.empty() && pathVersion!=level->getPathVersion())
			{
			// Przeliczenie sciezki
			LOG_DEBUG("Unit 0x%p, przeliczanie sciezki...");

			level->findPath(path, targetCollectible->getPosition());

			if(path.empty())
				{
				pathSearchCooldown=1.0f;

				const unsigned FW=level->getFieldWidth();
				const unsigned FH=level->getFieldHeight();

				const float OX=(rand()/(RAND_MAX+1.0f)*FW-FW/2.0f)*0.75f;
				const float OY=(rand()/(RAND_MAX+1.0f)*FH-FH/2.0f)*0.75f;

				target=level->getFieldPosition({0, 0}) + Vector(OX, OY);

				return;
				}

			path.reverse();

			target=level->getFieldPosition(path.front());
			path.pop_front();
			}

		const float DISTANCE=speed*dt;

		if(VectorLength(position-targetCollectible->getPosition())<DISTANCE)
			{
			level->addResources(targetCollectible->collect());
			targetCollectible=nullptr;
			}
		else if(VectorLength(position-target)<DISTANCE)
			{
			const unsigned FW=level->getFieldWidth();
			const unsigned FH=level->getFieldHeight();

			const float OX=(rand()/(RAND_MAX+1.0f)*FW-FW/2.0f)*0.25f;
			const float OY=(rand()/(RAND_MAX+1.0f)*FH-FH/2.0f)*0.25f;

			target=level->getFieldPosition(path.front()) + Vector(OX, OY);
			path.pop_front();
			}
		else
			{
			const Level::Field* field=((const Level*)level)->getField(level->getPositionOnField(target));

			if(field && field->turret && !field->turret->isWalkable())
				{
				target=position;
				return;
				}

			position=position + VectorNormalize(target-position)*DISTANCE;
			}*/

		if(VectorLength(position-targetCollectible->getPosition())<DISTANCE)
			{
			level->addResources(targetCollectible->collect());
			targetCollectible=nullptr;

			collectCooldown=1.0f;

			target=level->getFieldPosition({0, 0});
			}
		else
			{
			position=position + VectorNormalize(target-position)*DISTANCE;
			}
		}
	else
		{
		if(level->getUnlockedCollectiblesCount()>0u)
			{
			targetCollectible=level->findUnlockedCollectible();
			target=targetCollectible->getPosition();
			}

		position=position + VectorNormalize(target-position)*DISTANCE;

		const VectorI fieldXY=level->getPositionOnField(position);

		if(fieldXY.x==0 && fieldXY.y==0)
			{
			hp=0.0f;
			}
		}
	}

void UPlayerAcolyte::clear()
	{
	//
	}


void UPlayerAcolyte::damage(DamageType /*dmgType*/, float /*dmg*/)
	{
	return;
	}
