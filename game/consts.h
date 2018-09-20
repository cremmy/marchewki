/*
 * consts.h
 *
 *  Created on: 12 wrz 2018
 *      Author: crm
 */

#pragma once

// TODO pozamieniać stałe na poniższe wartości

namespace Game
	{
	/*****************************************************************************/
	/**** Koszty zasobów *********************************************************/
	/*****************************************************************************/
	namespace TurretBaseCostConstruction
		{
		const float PLAYER_CARROT_FIELD=         0.0f;
		const float PLAYER_UNIT_SINGLE_TARGET=   5.0f;
		const float PLAYER_UNIT_AREA_OF_EFFECT= 25.0f;
		const float PLAYER_UNIT_MINE=            2.0f;
		}
	namespace TurretBaseCostUpgrade
		{
		const float PLAYER_CARROT_FIELD=         0.0f;
		const float PLAYER_UNIT_SINGLE_TARGET=   4.0f;
		const float PLAYER_UNIT_AREA_OF_EFFECT= 12.5f;
		const float PLAYER_UNIT_MINE=            1.0f;
		}
	namespace TurretBaseCostRemoval
		{
		const float PLAYER_CARROT_FIELD=         0.0f;
		const float PLAYER_UNIT_SINGLE_TARGET=  -2.5f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=-12.5f;
		const float PLAYER_UNIT_MINE=           10.0f;
		}
	namespace TurretBaseResourceDrain
		{
		const float PLAYER_BASE=                 0.005f;
		const float PLAYER_CARROT_FIELD=         0.0f;
		const float PLAYER_UNIT_SINGLE_TARGET=   0.01f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  0.05f;
		const float PLAYER_UNIT_MINE=           -0.1f;
		}

	const float FIELD_RESOURCE_DRAIN=            0.005f; // VAL * resources * empty fields owned by player

	/*****************************************************************************/
	/**** Obrażenia wież *********************************************************/
	/*****************************************************************************/
	namespace TurretBaseDamage
		{
		const float PLAYER_UNIT_SINGLE_TARGET=   1.00f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  0.75f;
		const float PLAYER_UNIT_MINE=            2.00f;
		}
	namespace TurretMaxDamage
		{
		const float PLAYER_UNIT_SINGLE_TARGET=   2.00f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  1.50f;
		const float PLAYER_UNIT_MINE=            1.00f;
		}
	/*****************************************************************************/
	/**** Zasięg wież ************************************************************/
	/*****************************************************************************/
	// VAL * level->getFieldDiagonal()
	/*namespace TurretBaseRange
		{
		const float PLAYER_UNIT_SINGLE_TARGET=   0.00f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  0.00f;
		const float PLAYER_UNIT_MINE=            0.00f;
		}
	namespace TurretMaxRange
		{
		const float PLAYER_UNIT_SINGLE_TARGET=   0.00f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  0.00f;
		const float PLAYER_UNIT_MINE=            0.00f;
		}*/

	const float TURRET_AREA_OF_EFFECT_EXPLOSION_MOD=0.5f;

	/*****************************************************************************/
	/**** Prędkości pocisków *****************************************************/
	/*****************************************************************************/
	// VAL * level->getFieldDiagonal()
	namespace ProjectileBaseSpeed
		{
		const float SINGLE_TARGET=   1.0f;
		const float AREA_OF_EFFECT=  0.5f;
		}
	namespace ProjectileMaxSpeed // Prędkość na maksymalnym poziomie ulepszenia
		{
		const float SINGLE_TARGET=   1.2f;
		const float AREA_OF_EFFECT=  0.75f;
		}
	/*****************************************************************************/
	/**** Poziom groźności wież **************************************************/
	/*****************************************************************************/
	namespace TurretThreat
		{
		const float PLAYER_CARROT_FIELD=        -1.0f;
		const float PLAYER_UNIT_SINGLE_TARGET=   1.0f;
		const float PLAYER_UNIT_AREA_OF_EFFECT=  2.5f;
		const float PLAYER_UNIT_MINE=            9.0f;
		}
	/*****************************************************************************/
	/**** Modyfikatory obrażeń jednostek *****************************************/
	/*****************************************************************************/
	namespace UnitDamageModifier
		{
		const float INFANTRY_SINGLE_TARGET=      1.0f;
		const float INFANTRY_AREA_OF_EFFECT=     1.0f;
		const float INFANTRY_AREA_OF_EFFECT_TRAP=1.25f;

		const float ARMORED_SINGLE_TARGET=       1.5f;
		const float ARMORED_AREA_OF_EFFECT=      0.25f;
		const float ARMORED_AREA_OF_EFFECT_TRAP= 0.75f;
		}

	const float UNIT_PLAYER_ACOLYTE_SPEED=            0.6f; // VAL * level->getFieldDiagonal()
	const float UNIT_PLAYER_ACOLYTE_COLLECT_COOLDOWN= 1.0f;
	const float UNIT_ENEMY_INFANTRY_EATING_COOLDOWN=  0.5f;

	const float SPAWNER_TIMEOUT_FIRST=          15.0f;
	const float SPAWNER_COOLDOWN_SPAWNING=      15.0f;
	const float SPAWNER_TIMEOUT_SPREAD=         45.0f;
	const float SPAWNER_COOLDOWN_SPREAD_PRE=    15.0f;
	const float SPAWNER_COOLDOWN_SPREAD_POST=   SPAWNER_COOLDOWN_SPAWNING*0.66f;
	const float SPAWNER_SPREAD_CHANCE=          1.0f;
	const float SPAWNER_COOLDOWN_OVERCHARGE=    60.0f;

	const int SPAWNER_SPREAD_DISTANCE=          2;
	}


