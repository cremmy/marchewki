/*
 * rules.h
 *
 *  Created on: 27 lip 2018
 *      Author: crm
 */

#pragma once

enum Rules
	{
	RULE_NO_RULES=              0x0,
	RULE_DRAIN_RESOURCES=       0x1<<0,
	RULE_ENEMY_SPAWN_UNITS=     0x1<<1,
	RULE_ENEMY_BUILD_TURRETS=   0x1<<2,
	RULE_ENEMY_AVOID_TURRETS=   0x1<<3,
	};


