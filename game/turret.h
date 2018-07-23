/*
 * turret.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include "../engine/graphics/spriteptr.h"

namespace Game
	{
	class Level;

	class Turret
		{
		protected:
			enum class Type
				{
				PLAYER_BASE,
				PLAYER_CARROT_FIELD,

				PLAYER_UNIT_SINGLE_TARGET,
				PLAYER_UNIT_AREA_OF_EFFECT,
				PLAYER_UNIT_MINE,

				ENEMY_SPAWNER,
				};

			Level* level;

			float hp;

			int upgrade;
			float cooldown;

		public:
			Turret(): level(nullptr), hp(1.0f), upgrade(0), cooldown(0.0f)
				{
				//
				}
			virtual ~Turret()
				{
				//
				}

			virtual bool isWalkable() const {return false;}
		};

	} /* namespace Game */
