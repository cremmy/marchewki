/*
 * pareaofeffect.h
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#pragma once

#include "../engine/math/math.h"

#include "projectile.h"

namespace Game
	{
	class PAreaOfEffect: public Projectile
		{
		protected:
			Engine::Math::Vector target;
			float damageRange;

			// Hermite Curve
			float pathCur;
			float pathStep;
			Engine::Math::Vector startPosition;

		public:
			PAreaOfEffect(const Engine::Math::Vector& target, float damageRange, float damage, float maxSpeed);
			virtual ~PAreaOfEffect();

			virtual bool init() override;
			virtual void update(float dt) override;
		};

	} /* namespace Game */
