/*
 * turret_type.cpp
 *
 *  Created on: 30 sie 2018
 *      Author: crm
 */

#include "turret_type.h"


float Game::getTurretConstructionCost(TurretType type)
	{
	switch(type)
		{
		case TurretType::PLAYER_CARROT_FIELD:        return  0.0f;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return  5.0f;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return 25.0f;
		case TurretType::PLAYER_UNIT_MINE:           return  2.0f;

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
		case TurretType::PLAYER_CARROT_FIELD:        return  0.0f;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return  4.0f;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return 12.5f;
		case TurretType::PLAYER_UNIT_MINE:           return  1.0f;

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
		case TurretType::PLAYER_CARROT_FIELD:        return  0.0f;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return  -2.5f;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return -12.5f;
		case TurretType::PLAYER_UNIT_MINE:           return  10.0f;

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
		case TurretType::PLAYER_BASE:                return  -(upgrade+1)*0.25f;
		case TurretType::PLAYER_CARROT_FIELD:        return   0.0f;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:  return   0.01f;
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT: return   0.05f;
		case TurretType::PLAYER_UNIT_MINE:           return   0.0f;

		default:
		//case TurretType::ENEMY_SPAWNER:
			return 0.0f;
		}
	}
