/*
 * plinear.h
 *
 *  Created on: 4 sie 2018
 *      Author: crm
 */

#pragma once

#include "../engine/math/math.h"

#include "projectile.h"

namespace Game
	{
	class Unit;

	class PLinear: public Projectile
		{
		protected:
			Unit* target;

		public:
			PLinear(Unit* target, DamageType damageType, float damage, float maxSpeed);
			virtual ~PLinear();

			virtual bool init();
			virtual void update(float dt);
		};

	} /* namespace Game */
