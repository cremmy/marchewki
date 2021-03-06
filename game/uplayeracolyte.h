/*
 * uplayeracolyte.h
 *
 *  Created on: 7 sie 2018
 *      Author: crm
 */

#pragma once

#include <list>

#include "unit.h"

namespace Game
	{
	class Collectible;

	class UPlayerAcolyte: public Unit
		{
		protected:
			Collectible* targetCollectible;

			float collectCooldown;

		public:
			UPlayerAcolyte(): targetCollectible(nullptr), collectCooldown(0.0f)
				{
				//
				}
			virtual ~UPlayerAcolyte()
				{
				//
				}

			virtual bool init(Level* level, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target);
			virtual void update(float dt);
			//virtual void print(float tinterp);
			virtual void clear();

			virtual void damage(DamageType dmgType, float dmg);

			virtual UnitType getType() const {return UnitType::PLAYER_ACOLYTE;}
		};

	} /* namespace Game */
