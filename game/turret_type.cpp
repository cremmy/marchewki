/*
 * turret_type.cpp
 *
 *  Created on: 30 sie 2018
 *      Author: crm
 */

#include "turret_type.h"
#include "consts.h"

float Game::getTurretConstructionCost(TurretType type)
	{
	switch(type)
		{
		case TurretType::PLAYER_CARROT_FIELD:        return TurretBaseCostConstruction::PLAYER_CARROT_FIELD;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return TurretBaseCostConstruction::PLAYER_UNIT_SINGLE_TARGET;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return TurretBaseCostConstruction::PLAYER_UNIT_AREA_OF_EFFECT;
		case TurretType::PLAYER_UNIT_MINE:           return TurretBaseCostConstruction::PLAYER_UNIT_MINE;

		default:
		//case TurretType::PLAYER_BASE:
		//case TurretType::ENEMY_SPAWNER:
			return 0.0f;
		}
	}

float Game::getTurretUpgradeCost(TurretType type, int /*upgrade*/)
	{
	switch(type)
		{
		case TurretType::PLAYER_CARROT_FIELD:        return TurretBaseCostUpgrade::PLAYER_CARROT_FIELD;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return TurretBaseCostUpgrade::PLAYER_UNIT_SINGLE_TARGET;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return TurretBaseCostUpgrade::PLAYER_UNIT_AREA_OF_EFFECT;
		case TurretType::PLAYER_UNIT_MINE:           return TurretBaseCostUpgrade::PLAYER_UNIT_MINE;

		default:
		//case TurretType::PLAYER_BASE:
		//case TurretType::ENEMY_SPAWNER:
			return 0.0f;
		}
	}

float Game::getTurretRemovalCost(TurretType type, int /*upgrade*/)
	{
	switch(type)
		{
		case TurretType::PLAYER_CARROT_FIELD:        return TurretBaseCostRemoval::PLAYER_CARROT_FIELD;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return TurretBaseCostRemoval::PLAYER_UNIT_SINGLE_TARGET;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return TurretBaseCostRemoval::PLAYER_UNIT_AREA_OF_EFFECT;
		case TurretType::PLAYER_UNIT_MINE:           return TurretBaseCostRemoval::PLAYER_UNIT_MINE;

		default:
		//case TurretType::PLAYER_BASE:
		//case TurretType::ENEMY_SPAWNER:
			return 0.0f;
		}
	}


float Game::getTurretResourceDrain(TurretType type, int upgrade)
	{
	switch(type)
		{
		case TurretType::PLAYER_BASE:                return TurretBaseResourceDrain::PLAYER_BASE*upgrade;
		case TurretType::PLAYER_CARROT_FIELD:        return TurretBaseResourceDrain::PLAYER_CARROT_FIELD;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return TurretBaseResourceDrain::PLAYER_UNIT_SINGLE_TARGET*(upgrade+1);
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return TurretBaseResourceDrain::PLAYER_UNIT_AREA_OF_EFFECT*(upgrade+1);
		case TurretType::PLAYER_UNIT_MINE:           return TurretBaseResourceDrain::PLAYER_UNIT_MINE*(upgrade+1);

		default:
		//case TurretType::ENEMY_SPAWNER:
			return 0.0f;
		}
	}
