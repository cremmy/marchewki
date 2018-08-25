/*
 * tcarrotfield.h
 *
 *  Created on: 25 sie 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{
	class TCarrotField: public Turret
		{
		protected:
			virtual bool updateFieldOwners() const override;

			bool panic;

		public:
			TCarrotField(): Turret(6), panic(false)
				{
				//
				}
			virtual ~TCarrotField()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			//virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_CARROT_FIELD;}
			virtual bool isWalkable() const override {return true;}
			virtual bool isFlat() const {return true;}
			virtual bool isUpgradable() const override {return false;}
			virtual bool isRemovable() const override {return true;}
			//virtual float getConstructionCost() const override {return 0.0f;}
			//virtual float getUpgradeCost() const override {return 0.0f;}
			//virtual float getRemovalCost() const override {return 0.0f;}
			//virtual float getRange() const {return 0.0f;}
			virtual float getCooldown() const;
		};

	} /* namespace Game */
