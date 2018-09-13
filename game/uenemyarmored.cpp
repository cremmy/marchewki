/*
 * uenemyarmored.cpp
 *
 *  Created on: 9 wrz 2018
 *      Author: crm
 */

#include "uenemyarmored.h"

/*
 * uinfantry.cpp
 *
 *  Created on: 26 lip 2018
 *      Author: crm
 */

#include "uenemyinfantry.h"

#include <algorithm>
#include <ctime>
#include <random>

#include "../engine/debug/log.h"

#include "consts.h"
#include "level.h"
#include "turret.h"

using namespace Game;

bool UEnemyArmored::init(Level* level, const Engine::Math::VectorI& fposition, const Engine::Math::VectorI& /*target*/)
	{
	this->level=level;

	this->position=level->getFieldPosition(fposition);
	this->target=this->position;

	if(!(sprite=Engine::Graphics::SpritePtr("sprite/unit_enemy_armored.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

void UEnemyArmored::update(float dt)
	{
	using namespace Engine::Math;

	const int FIELDS_BEFORE_COOLDOWN=4;
	const int FIELDS_BEFORE_COOLDOWN_INCREMENT=2;
	const int MAX_COOLDOWN_MULTIPLIER=10;
	const float COOLDOWN_PER_FIELD=0.25f;

	const VectorI fieldXY=level->getPositionOnField(position);
	//const Level::Field* curField=level->getField(fieldXY.x, fieldXY.y);

	sprite.update(dt);

	if(!isAlive())
		{
		return;
		}

	if(damageTimer>0.0f)
		{
		damageTimer-=dt;
		}

	// Dojście do celu
	//if(curField->turret->getType()==TurretType::PLAYER_BASE) // W sumie to czemu nie było to tak sprawdzane?
	if(fieldXY.x==0 && fieldXY.y==0)
		{
		hp=0.0f;

		Turret* turret=((const Level*)level)->getField(fieldXY)->turret;
		if(turret)
			turret->setHP(turret->getHP()-1.0f);

		return;
		}

	const float DISTANCE=speed*dt;

	// Dojście do aktualnego checkpointa
	if(cooldownWalking>0.0f)
		{
		cooldownWalking-=dt;
		}
	else if(VectorLength(position-target)<DISTANCE)
		{
		const unsigned FW=level->getFieldWidth();
		const unsigned FH=level->getFieldHeight();

		const float OX=(rand()/(RAND_MAX+1.0f)*FW-FW/2.0f)*0.45f;
		const float OY=(rand()/(RAND_MAX+1.0f)*FH-FH/2.0f)*0.45f;

		const VectorI nextFieldXY=level->findPath(fieldXY);
		target=level->getFieldPosition(nextFieldXY) + Vector(OX, OY);

		++fieldsWalked;
		if(FIELDS_BEFORE_COOLDOWN>4)
			{
			cooldownWalking=std::min((fieldsWalked-FIELDS_BEFORE_COOLDOWN)/FIELDS_BEFORE_COOLDOWN_INCREMENT+1, MAX_COOLDOWN_MULTIPLIER) * COOLDOWN_PER_FIELD;
			}
		}
	// Wędrówka...
	else
		{
		sprite.setAnimation(1);
		const Level::Field* field=((const Level*)level)->getField(level->getPositionOnField(target));

		if(field && field->turret && !field->turret->isWalkable())
			{
			target=position;
			return;
			}

		position=position + VectorNormalize(target-position)*DISTANCE;
		}
	}

void UEnemyArmored::clear()
	{
	//
	}


void UEnemyArmored::damage(DamageType dmgType, float dmg)
	{
	switch(dmgType)
		{
		case DamageType::SINGLE_TARGET:
			hp-=dmg*UnitDamageModifier::ARMORED_SINGLE_TARGET;
			damageTimer=0.5f*UnitDamageModifier::INFANTRY_AREA_OF_EFFECT;
		break;

		case DamageType::AOE:
			hp-=dmg*UnitDamageModifier::ARMORED_AREA_OF_EFFECT;
			damageTimer=0.5f*UnitDamageModifier::ARMORED_AREA_OF_EFFECT;
		break;

		case DamageType::AOE_TRAP:
			hp-=dmg*UnitDamageModifier::ARMORED_AREA_OF_EFFECT_TRAP;
			damageTimer=0.5f*UnitDamageModifier::ARMORED_AREA_OF_EFFECT_TRAP;
		break;

		default:
			hp-=dmg;
			damageTimer=0.5f;
		break;
		}

	if(hp<=0.0f)
		{
		level->spawnCollectible(position, maxhp);
		}
	}

