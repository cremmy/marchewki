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

			int wave;

			/*Engine::Graphics::SpritePtr spriteEnemy;*/

		public:
			TSpawner(): Turret(), wave(0)
				{
				//
				}
			virtual ~TSpawner()
				{
				//
				}

			virtual bool init() override;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition) override;
			virtual bool removeFromLevel() override;
			virtual void update(float dt) override;
			//virtual void print(float tinterp) override;

			virtual TurretType getType() const override {return TurretType::ENEMY_SPAWNER;}
			virtual bool isWalkable() const override {return true;}
//			virtual bool isUpgradable() const override {return false;}
//			virtual bool isRemovable() const override {return false;}
//			virtual float getConstructionCost() const override {return 0.0f;}
//			virtual float getUpgradeCost() const override {return 0.0f;}
//			virtual float getRemovalCost() const override {return 0.0f;}
//			virtual float getRange() const override {return 0.0f;}
//			virtual float getCooldown() const override {return 0.0f;}
		};

	} /* namespace Game */
