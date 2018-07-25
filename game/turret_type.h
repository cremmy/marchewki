/*
 * turret_type.h
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#pragma once

namespace Game
	{
	enum class TurretType
		{
		PLAYER_BASE,
		PLAYER_CARROT_FIELD,

		PLAYER_UNIT_SINGLE_TARGET,
		PLAYER_UNIT_AREA_OF_EFFECT,
		PLAYER_UNIT_MINE,

		ENEMY_SPAWNER,
		};
	}


