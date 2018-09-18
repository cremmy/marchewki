/*
 * uplayeracolyte.cpp
 *
 *  Created on: 7 sie 2018
 *      Author: crm
 */

#include "uplayeracolyte.h"

#include "../engine/debug/log.h"

#include "consts.h"
#include "collectible.h"
#include "level.h"
#include "particleemitter.h"
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

		if(collectCooldown<=0.0f)
			sprite.setAnimation(0);

		return;
		}

	const float DISTANCE=speed*dt;

	if(targetCollectible)
		{
		if(VectorLength(position-targetCollectible->getPosition())<DISTANCE)
			{
			level->addResources(targetCollectible->collect());
			targetCollectible=nullptr;

			collectCooldown=UNIT_PLAYER_ACOLYTE_COLLECT_COOLDOWN;
			sprite.setAnimation(1);
			level->addEmitter(new ParticleEmitter(ParticleEmitterType::LINEAR_UP, position, 0.5f, Engine::Graphics::SpritePtr("sprite/particle_green.xml"), 100, 8, 200, 250, 1.0f));
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
		else
			{
			target=level->getFieldPosition({0, 0});
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
