/*
 * uenemyarmored.h
 *
 *  Created on: 9 wrz 2018
 *      Author: crm
 */

#pragma once

#include "unit.h"

namespace Game
	{
	class UEnemyArmored: public Unit
		{
		protected:
			int fieldsWalked;
			float cooldownWalking;

		public:
			UEnemyArmored(): Unit(), fieldsWalked(0), cooldownWalking(0.0f)
				{
				//
				}
			virtual ~UEnemyArmored()
				{
				//
				}

			virtual bool init(Level* level, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target);
			virtual void update(float dt);
			//virtual void print(float tinterp);
			virtual void clear();

			virtual void damage(DamageType dmgType, float dmg);

			virtual UnitType getType() const {return UnitType::ENEMY_ARMORED;}
		};

	} /* namespace Game */
