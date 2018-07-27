/*
 * unit.h
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#pragma once

#include "../engine/graphics/spriteptr.h"
#include "../engine/math/math.h"

//#include "level.h"
#include "damage_type.h"
#include "unit_type.h"

namespace Game
	{
	class Level;

	class Unit
		{
		protected:
			Level* level;

			Engine::Math::Vector position;
			Engine::Math::Vector target;
			float hp;
			float speed;

			Engine::Graphics::SpritePtr sprite;

		public:
			Unit(): level(nullptr), position(), hp(1.0f), speed(1.0f)
				{
				//
				}
			virtual ~Unit()
				{
				//
				}

			virtual bool init(Level* level, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target)=0;
			virtual void update(float dt)=0;
			virtual void print(float tinterp)=0;
			virtual void clear()=0;

			virtual void damage(DamageType dmgType, float dmg)=0;

			virtual UnitType getType() const=0;
			bool isAlive() const {return hp>0.0f;}
			Engine::Math::Vector getPosition() const {return position;}
			float getHP() const {return hp;}
			float getSpeed() const {return speed;}

			void setHP(float s) {hp=s;}
			void setSpeed(float s) {speed=s;}
		};

	} /* namespace Game */
