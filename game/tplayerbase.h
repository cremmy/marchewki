/*
 * tplayerbase.h
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{
	class TPlayerBase: public Turret
		{
		protected:
			virtual bool updateFieldOwners() const override;

		public:
			TPlayerBase(): Turret(5)
				{
				//
				}
			virtual ~TPlayerBase()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
//			virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::PLAYER_BASE;}
			virtual bool isWalkable() const override {return true;}
//			virtual bool isFlat() const {return false;}
//			virtual bool isUpgradable() const override {return false;}
//			virtual bool isRemovable() const override {return false;}
			virtual float getResourceDrain() const override;
			virtual float getCooldown() const override;
		};

	} /* namespace Game */
