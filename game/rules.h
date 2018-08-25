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
	RULE_BUILDING_COST=         0x1<<0, // Darmowe budowanie
	RULE_ENEMY_SPAWN_UNITS=     0x1<<1, // Wrogowie się nie pojawiają
	RULE_GOD_MODE=              0x1<<2, // Gracz nie traci HP
	RULE_DRAIN_RESOURCES=       0x1<<3, // Wieże nie zużywają zasobów
	RULE_ENEMY_BUILD_TURRETS=   0x1<<4, // Nie pojawiają się kolejne gniazda wrogów
	RULE_ENEMY_PREFER_CARROTS=  0x1<<5, // Wrogowie nie idą do najbliższego pola marchewkowego

	RULE_PLAYER_STOMPS_CARROTS= 0x1<<6, // Jednostki gracza mogą chodzić po polu marchewkowym bez niszczenia go
	};
