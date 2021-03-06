/*
 * tmine.h
 *
 *  Created on: 21 sie 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{
	class TMine: public Turret
		{
		protected:
			virtual bool updateFieldOwners() override;

			bool ready;

		public:
			TMine(): Turret(4), ready(true)
				{
				//
				}
			virtual ~TMine()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			//virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_UNIT_MINE;}
			virtual bool isWalkable() const override {return true;}
			virtual bool isFlat() const {return true;}
			virtual bool isUpgradable() const override {return true;}
			virtual bool isRemovable() const override {return true;}
			virtual float getResourceDrain() const override;
			virtual float getRange() const override;
			virtual float getCooldown() const override;
		};

	} /* namespace Game */
