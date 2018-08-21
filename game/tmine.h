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
			virtual bool updateFieldOwners() const override;

		public:
			TMine(): Turret(4)
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

			virtual TurretType getType() const override {return TurretType::PLAYER_UNIT_SINGLE_TARGET;}
			virtual bool isWalkable() const override {return true;}
			virtual bool isFlat() const {return true;}
			virtual bool isUpgradable() const override {return true;}
			virtual bool isRemovable() const override {return true;}
			virtual float getConstructionCost() const override {return 2.0f;}
			virtual float getUpgradeCost() const override {return 5.0f;}
			virtual float getRemovalCost() const override {return 15.0f;}
			virtual float getRange() const;
			virtual float getCooldown() const;
		};

	} /* namespace Game */
