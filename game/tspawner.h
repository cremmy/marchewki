/*
 * tspawner.h
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#pragma once

#include "turret.h"

namespace Game
	{

	class TSpawner: public Turret
		{
		protected:
			virtual bool updateFieldOwners() const override;

		public:
			TSpawner()
				{
				//
				}
			virtual ~TSpawner()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, int x, int y) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			virtual void print(float tinterp) override;

			virtual bool isWalkable() const override {return true;}
			virtual TurretType getType() const override {return TurretType::PLAYER_BASE;}
		};

	} /* namespace Game */
