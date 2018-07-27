/*
 * uinfantry.cpp
 *
 *  Created on: 26 lip 2018
 *      Author: crm
 */

#include "uenemyinfantry.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "level.h"
#include "turret.h"

using namespace Game;

bool UEnemyInfantry::init(Level* level, const Engine::Math::VectorI& position, const Engine::Math::VectorI& /*target*/)
	{
	this->level=level;

	this->position=level->getFieldPosition(position.x, position.y);
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

	//if(curField->turret->getType()==TurretType::PLAYER_BASE)
	if(fieldXY.x==0 && fieldXY.y==0)
		{
		hp=0.0f;
		return;
		}

	const float DISTANCE=speed*dt;

	if(VectorLength(position-target)<DISTANCE)
		{
		const unsigned FW=level->getFieldWidth();
		const unsigned FH=level->getFieldHeight();

		const float OX=(rand()/(RAND_MAX+1.0f)*FW-FW/2.0f)*0.45f;
		const float OY=(rand()/(RAND_MAX+1.0f)*FH-FH/2.0f)*0.45f;

		const VectorI nextFieldXY=level->findPath(fieldXY.x, fieldXY.y);
		target=level->getFieldPosition(nextFieldXY.x, nextFieldXY.y) + Vector(OX, OY);
		}
	else
		{
		position=position + VectorNormalize(target-position)*DISTANCE;
		}
	}

void UEnemyInfantry::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	Render::getInstance().draw(cam.getBillboard(position), sprite);
	}

void UEnemyInfantry::clear()
	{
	//
	}


void UEnemyInfantry::damage(DamageType dmgType, float dmg)
	{
	/*switch(dmgType)
		{
		default:
		case DamageType::SINGLE_TARGET:
		case DamageType::AOE:
			hp-=dmg;
		break;
		}*/

	hp-=dmg;
	}

