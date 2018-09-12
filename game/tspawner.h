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
		public:
			struct WaveDef
				{
				int uinfantry;
				int uarmored;
				float hp;
				float cooldownUnit;
				float cooldownWave;
				int level;
				};

		protected:
			enum State
				{
				STATE_PRE_SPAWN,
				STATE_SPAWNING,
				STATE_NORMAL,
				STATE_ATTACKED,
				STATE_OVERLOAD,
				};

			virtual bool updateFieldOwners() const override;

			State state;
			int wave;
			int waveUnit;
			const WaveDef* waveCurDef;

			/*Engine::Graphics::SpritePtr spriteEnemy;*/

			void initStateSpawning();
			void initStateNormal();
			void initStateAttacked();
			void initStateOverload();

			void updateStateSpawning(float dt);
			void updateStateNormal(float dt);
			void updateStateAttacked(float dt);
			void updateStateOverload(float dt);

		public:
			TSpawner(): Turret(), state(STATE_PRE_SPAWN), wave(0), waveUnit(0), waveCurDef(nullptr)
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
			virtual bool isFlat() const {return true;}
//			virtual bool isUpgradable() const override {return false;}
//			virtual bool isRemovable() const override {return false;}
//			virtual float getRange() const override {return 0.0f;}
//			virtual float getCooldown() const override {return 0.0f;}
		};

	} /* namespace Game */
