/*
 * uinfantry.h
 *
 *  Created on: 26 lip 2018
 *      Author: crm
 */

#pragma once

#include "unit.h"

namespace Game
	{
	class UEnemyInfantry: public Unit
		{
		public:
			UEnemyInfantry(): Unit()
				{
				//
				}
			virtual ~UEnemyInfantry()
				{
				//
				}

			virtual bool init(Level* level, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target);
			virtual void update(float dt);
			virtual void print(float tinterp);
			virtual void clear();

			virtual void damage(DamageType dmgType, float dmg);

			virtual UnitType getType() const {return UnitType::ENEMY_INFANTRY;}
		};

	} /* namespace Game */
