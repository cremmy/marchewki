/*
 * turret.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include "../engine/graphics/spriteptr.h"

#include "turret_type.h"

namespace Game
	{
	class Level;
	class Unit;

	class Turret
		{
		protected:
			bool init(Level* level, const Engine::Math::VectorI& fposition);

			Level* level;

			// Pozycja w poziomie (numer pola)
			Engine::Math::VectorI fposition;

			float hp;

			int upgrade;
			const int MAX_UPGRADE;

			float cooldown;

			Engine::Graphics::SpritePtr sprite;

			Unit* target;

		public:
			Turret(int maxUpgrade=0): level(nullptr), fposition(-1, -1), hp(1.0f), upgrade(0), MAX_UPGRADE(maxUpgrade), cooldown(0.0f), target(nullptr)
				{
				//
				}
			virtual ~Turret()
				{
				//
				}

			virtual bool init()=0;
			virtual bool attachToLevel(Level* level, const Engine::Math::VectorI& fposition)=0;
			virtual bool updateFieldOwners() const=0;
			virtual bool removeFromLevel()=0;
			virtual void update(float dt)=0;
			virtual void print(float tinterp);

			virtual TurretType getType() const=0;
			virtual bool isWalkable() const {return false;}
			virtual bool isFlat() const {return false;}
			virtual bool isUpgradable() const {return false;}
			virtual bool isRemovable() const {return false;}
			virtual float getConstructionCost() const {return 0.0f;}
			virtual float getUpgradeCost() const {return 0.0f;}
			virtual float getRemovalCost() const {return 0.0f;}
			virtual float getRange() const {return 0.0f;}
			virtual float getCooldown() const {return 0.0f;}
			// TODO virtual float getResourceDrain() const {return 0.0f;};
			bool isReady() const {return cooldown<=0.0f;}
			bool isAlive() const {return hp>0.0f;}
			int getPositionX() const {return fposition.x;}
			int getPositionY() const {return fposition.y;}
			float getHP() const {return hp;}
			int getUpgrade() const {return upgrade;}
			const Engine::Graphics::SpritePtr& getSprite() const {return sprite;}

			void setHP(float s) {hp=s;}
			void setUpgrade(int s) {if(s<0 || s>MAX_UPGRADE) return; else upgrade=s; updateFieldOwners();}
			void incUpgrade() {if(upgrade>MAX_UPGRADE) return; else upgrade+=1; updateFieldOwners();}
			void setTarget(Unit* target);
		};

	} /* namespace Game */
