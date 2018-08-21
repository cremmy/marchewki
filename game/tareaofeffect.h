/*
 * tareaofeffect.h
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{
	class TAreaOfEffect: public Turret
		{
		protected:
			virtual bool updateFieldOwners() const override;

			Engine::Graphics::SpritePtr spriteProjectile;

		public:
			TAreaOfEffect(): Turret(3)
				{
				//
				}
			virtual ~TAreaOfEffect()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_UNIT_AREA_OF_EFFECT;}
			virtual bool isWalkable() const override {return false;}
			virtual bool isUpgradable() const override {return true;}
			virtual bool isRemovable() const override {return true;}
			virtual float getConstructionCost() const override {return 25.0f;}
			virtual float getUpgradeCost() const override {return 75.0f;}
			virtual float getRemovalCost() const override {return -12.5f;}
			virtual float getRange() const;
			virtual float getCooldown() const;
		};

	} /* namespace Game */
