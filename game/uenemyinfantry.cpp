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
#include "rules.h"
#include "turret.h"

using namespace Game;

// Global, bueee
auto rng = std::default_random_engine(time(nullptr));

bool UEnemyInfantry::init(Level* level, const Engine::Math::VectorI& fposition, const Engine::Math::VectorI& /*target*/)
	{
	this->level=level;

	this->position=level->getFieldPosition(fposition);
	this->target=this->position;//level->getFieldPosition(target.x, target.y);
	// Target ignorowany, jednostki domyślnie idą do 0,0

	if(!(sprite=Engine::Graphics::SpritePtr("sprite/unit_enemy_infantry.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	return true;
	}

void UEnemyInfantry::update(float dt)
	{
	using namespace Engine::Math;

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
		if(isRuleEnabled(RULE_DRAIN_HP) && turret)
			turret->setHP(turret->getHP()-1.0f);

		return;
		}

	const float DISTANCE=speed*dt;

	// Dojście do aktualnego checkpointa
	if(cooldownEating>0.0f)
		{
		cooldownEating-=dt;
		}
	else if(VectorLength(position-target)<DISTANCE)
		{
		const unsigned FW=level->getFieldWidth();
		const unsigned FH=level->getFieldHeight();

		const float OX=(rand()/(RAND_MAX+1.0f)*FW-FW/2.0f)*0.45f;
		const float OY=(rand()/(RAND_MAX+1.0f)*FH-FH/2.0f)*0.45f;

		auto isFieldACarrotField=[this](const VectorI& fposition)->bool
			{
			const Level::Field* field=((const Level*)level)->getField(fposition);

			if(!field)
				return false;

			const Turret* turret=field->turret;

			if(!turret)
				return false;
			if(turret->getType()!=TurretType::PLAYER_CARROT_FIELD)
				return false;
			if(turret->getUpgrade()<1) // Olewamy pola marchewkowe bez marchewek
				return false;

			return true;
			};

		auto isAnyNeighbourACarrotField=[this, OX, OY, isFieldACarrotField](const VectorI& fposition, Vector& target)->bool
			{
			std::vector<VectorI> neighbours(
				{
				fposition+VectorI(-1,  0),
				fposition+VectorI( 1,  0),
				fposition+VectorI( 0, -1),
				fposition+VectorI( 0,  1)
				});

			std::shuffle(neighbours.begin(), neighbours.end(), rng);

			for(auto& fieldXY: neighbours)
				{
				if(!isFieldACarrotField(fieldXY))
					continue;

				target=level->getFieldPosition(fieldXY)+Vector(OX, OY);

				return true;
				}

			return false;
			};

		if(isFieldACarrotField(fieldXY))
			{
			cooldownEating=UNIT_ENEMY_INFANTRY_EATING_COOLDOWN;
			sprite.setAnimation(0);

			target=level->getFieldPosition(fieldXY) + Vector(OX, OY);
			}
		else if(((const Level*)level)->getField({0, 0})->turret->getHP()<=0.0)
			{
			VectorI next=VectorClamp(VectorI(0, 0), fieldXY+VectorI(1-2*(rand()%2), 1-2*(rand()%2)), VectorI(level->getWidth()-1, level->getHeight()-1));
			target=level->getFieldPosition(next) + Vector(OX, OY);
			}
		else if(isRuleEnabled(RULE_ENEMY_PREFER_CARROTS) && isAnyNeighbourACarrotField(fieldXY, target))
			{
			//
			}
		else
			{
			const VectorI nextFieldXY=level->findPath(fieldXY);
			target=level->getFieldPosition(nextFieldXY) + Vector(OX, OY);
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

/*void UEnemyInfantry::print(float tinterp)
	{
	const Vector directionRelative=VectorNormalize(cam.getView()*direction);
	const float angle=(directionRelative.x>0.0f)?(360.0f-acos(directionRelative.y)*180.0f/M_PI) : (acos(directionRelative.y)*180.0f/M_PI);

	sprite.setDirection(Engine::Graphics::Sprite::DirectionName::UP+angle);

	Unit::print(tinterp);
	}*/

void UEnemyInfantry::clear()
	{
	//
	}


void UEnemyInfantry::damage(DamageType dmgType, float dmg)
	{
	//const float prevhp=hp;
	switch(dmgType)
		{
		case DamageType::SINGLE_TARGET:
			hp-=dmg*UnitDamageModifier::INFANTRY_SINGLE_TARGET;
			damageTimer=0.5f*UnitDamageModifier::INFANTRY_SINGLE_TARGET;
		break;

		case DamageType::AOE:
			hp-=dmg*UnitDamageModifier::INFANTRY_AREA_OF_EFFECT;
			damageTimer=0.5f*UnitDamageModifier::INFANTRY_AREA_OF_EFFECT;
		break;

		case DamageType::AOE_TRAP:
			hp-=dmg*UnitDamageModifier::INFANTRY_AREA_OF_EFFECT_TRAP;
			damageTimer=0.5f*UnitDamageModifier::INFANTRY_AREA_OF_EFFECT_TRAP;
		break;

		default:
			hp-=dmg;
			damageTimer=0.5f;
		break;
		}

	//LOG_DEBUG("Damaged: %.2f/%.2f (%.2f->%.2f)", prevhp-hp, dmg, prevhp, hp);

	if(hp<=0.0f)
		{
		//level->spawnCollectible(position, 1.0f);
		level->spawnCollectible(position, maxhp);
		}
	}

