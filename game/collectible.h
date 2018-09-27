/*
 * collectible.h
 *
 *  Created on: 7 sie 2018
 *      Author: crm
 */

#pragma once

#include "../engine/math/math.h"
#include "../engine/graphics/spriteptr.h"

#include "consts.h"

namespace Game
	{
	class Collectible
		{
		protected:
			Engine::Math::Vector position;
			float value;

			Engine::Graphics::SpritePtr sprite;

			bool locked;
			bool alive;
			bool collected;

			float timeout;

		public:
			Collectible(): value(0.0f), locked(false), alive(true), collected(false), timeout(COLLECTIBLE_TIMEOUT)
				{
				//
				}
			virtual ~Collectible()
				{
				//
				}

			bool init(const Engine::Math::Vector& position, float value);
			void update(float dt);
			void print(float tinterp);

			bool isLocked() const {return locked;}
			bool isAlive() const {return alive;}
			Engine::Math::Vector getPosition() const {return position;}

			void lock() {locked=true;}
			float collect() {if(collected) return 0.0f; collected=true; return value;}
		};

	} /* namespace Game */
