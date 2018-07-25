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

	class Turret
		{
		protected:
			bool init(Level* level, int x, int y);

			Level* level;

			// Pozycja w poziomie (numer pola)
			int x;
			int y;

			float hp;

			int upgrade;
			const int MAX_UPGRADE;

			float cooldown;

			Engine::Graphics::SpritePtr sprite;

		public:
			Turret(int maxUpgrade=0): level(nullptr), x(-1), y(-1), hp(1.0f), upgrade(0), MAX_UPGRADE(maxUpgrade), cooldown(0.0f)
				{
				//
				}
			virtual ~Turret()
				{
				//
				}

			virtual bool init()=0;
			virtual bool attachToLevel(Level* level, int x, int y)=0;
			virtual bool updateFieldOwners() const=0;
			virtual bool removeFromLevel()=0;
			virtual void update(float dt)=0;
			virtual void print(float tinterp)=0;

			virtual bool isWalkable() const {return false;}
			virtual TurretType getType() const=0;
			bool isReady() const {return cooldown<=0.0f;}
			bool isAlive() const {return hp>0.0f;}
			int getPositionX() const {return x;}
			int getPositionY() const {return y;}
			float getHP() const {return hp;}
			int getUpgrade() const {return upgrade;}
			const Engine::Graphics::SpritePtr& getSprite() const {return sprite;}

			void setHP(float s) {hp=s;}
			void setUpgrade(int s) {if(s<0 || s>MAX_UPGRADE) return; else upgrade=s;}
			void incUpgrade() {if(upgrade>MAX_UPGRADE) return; else upgrade+=1;}
		};

	} /* namespace Game */
