/*
 * tsingletarget.h
 *
 *  Created on: 1 sie 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{
	class TSingleTarget: public Turret
		{
		protected:
			virtual bool updateFieldOwners() const override;

			Engine::Graphics::SpritePtr spriteProjectile;

		public:
			TSingleTarget(): Turret(3)
				{
				//
				}
			virtual ~TSingleTarget()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			//virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_BASE;}
			virtual bool isWalkable() const override {return false;}
			virtual bool isUpgradable() const override {return true;}
			virtual bool isRemovable() const override {return true;}
			virtual float getConstructionCost() const override {return 5.0f;}
			virtual float getUpgradeCost() const override {return 25.0f;}
			virtual float getRemovalCost() const override {return -2.5f;}
			virtual float getRange() const;
			virtual float getCooldown() const;
		};
	} /* namespace Game */
