/*
 * level.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include <vector>

#include "../engine/graphics/spriteptr.h"

namespace Game
	{
	class Turret;

	class Level
		{
		protected:
			struct Field
				{
				Field(): sprite(), turret(nullptr), collectibles() {}

				Engine::Graphics::SpritePtr sprite;

				Turret* turret;
				std::vector<void*> collectibles;
				};

			// field[Y][X]
			std::vector<std::vector<Field*>> field;

		public:
			Level()
				{
				//
				}
			virtual ~Level()
				{
				//
				}

			bool init(unsigned w, unsigned h);
			void update(float dt);
			void print(float tinterp);
			void clear();

			bool resize(unsigned w, unsigned h);
			bool resizeIncreaseXByOne();
			bool resizeIncreaseYByOne();

			Field* getField(unsigned x, unsigned y);
			const Field* getField(unsigned x, unsigned y) const;
			unsigned getWidth() const {return field.empty()?0u:field[0u].size();}
			unsigned getHeight() const {return field.size();}
		};

	} /* namespace Game */
