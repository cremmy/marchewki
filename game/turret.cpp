/*
 * turret.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "turret.h"

using namespace Game;

bool Turret::init(Level* level, const Engine::Math::VectorI& fposition)
	{
	this->level=level;
	this->fposition=fposition;

	return true;
	}
