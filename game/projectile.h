/*
 * projectile.h
 *
 *  Created on: 4 sie 2018
 *      Author: crm
 */

#pragma once

#include "../engine/math/math.h"
#include "../engine/graphics/spriteptr.h"

#include "damage_type.h"

namespace Game
	{
	class Level;
	class Unit;

	class Projectile
		{
		protected:
			bool alive;

			float maxSpeed;
			DamageType damageType;
			float damage;

			Engine::Math::Vector position;
			Engine::Math::Vector direction;

			Engine::Graphics::SpritePtr sprite;

			Level* level;

		public:
			Projectile(DamageType damageType, float damage, float maxSpeed): alive(true), maxSpeed(maxSpeed), damageType(damageType), damage(damage), level(nullptr)
				{
				//
				}
			virtual ~Projectile()
				{
				//
				}

			virtual bool init()=0;
			virtual void update(float dt)=0;
			virtual void print(float tinterp);

			bool isAlive() const {return alive;}

			void setPosition(const Engine::Math::Vector& position) {this->position=position;}
			void setLevel(Level* s) {level=s;}
		};

	} /* namespace Game */
