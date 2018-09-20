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
				STATE_SPREADING,
				STATE_SPREADING_POST,
				STATE_PANIC,
				STATE_OVERCHARGE,

				STATE_FINISHED,
				};

			virtual bool updateFieldOwners() override;

			State state;
			int wave;
			int waveUnit;
			const WaveDef* waveCurDef;

			float spreadCooldown;
			int spreadCount;

			float overchargeCooldown;

			/*Engine::Graphics::SpritePtr spriteEnemy;*/

			void initStateSpawning();
			void initStateNormal();
			void initStateSpreading();
			void initStatePanic();
			void initStateOvercharge();
			void initStateFinished();

			void updateStateSpawning(float dt);
			void updateStateNormal(float dt);
			void updateStateSpreading(float dt);
			void updateStatePanic(float dt);
			void updateStateOvercharge(float dt);

			bool canSpread();
			static bool canSpreadToField(const Level* level, const Engine::Math::VectorI& fposition);
			static int getNearbySpawnerCount(const Level* level, const Engine::Math::VectorI& fposition);

		public:
			TSpawner(): Turret(), state(STATE_PRE_SPAWN), wave(0), waveUnit(0), waveCurDef(nullptr), spreadCooldown(0.0f), spreadCount(0), overchargeCooldown(0.0f)
				{
				//
				}
			virtual ~TSpawner()
				{
				//
				}

			static bool isFieldSafe(const Level* level, const Engine::Math::VectorI& fposition);
			static bool isFieldExtraSafe(const Level* level, const Engine::Math::VectorI& fposition);

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
