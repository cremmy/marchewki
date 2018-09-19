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
			virtual bool updateFieldOwners() override;

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
			//virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_UNIT_AREA_OF_EFFECT;}
			virtual bool isWalkable() const override {return false;}
			virtual bool isFlat() const {return true;}
			virtual bool isUpgradable() const override {return true;}
			virtual bool isRemovable() const override {return true;}
			virtual float getRange() const;
			virtual float getCooldown() const;
		};

	} /* namespace Game */
