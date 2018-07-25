/*
 * turret.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "turret.h"

using namespace Game;

bool Turret::init(Level* level, int x, int y)
	{
	this->level=level;
	this->x=x;
	this->y=y;

	return true;
	}
