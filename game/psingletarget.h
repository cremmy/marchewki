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

	class PSingleTarget: public Projectile
		{
		protected:
			Unit* target;

		public:
			PSingleTarget(Unit* target, float damage, float maxSpeed);
			virtual ~PSingleTarget();

			virtual bool init() override;
			virtual void update(float dt) override;
		};

	} /* namespace Game */
