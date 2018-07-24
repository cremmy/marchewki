/*
 * level.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include <vector>

#include "../engine/graphics/imageptr.h"
#include "../engine/graphics/spriteptr.h"

namespace Game
	{
	class Turret;

	class Level
		{
		protected:
			struct Field
				{
				enum class Owner
					{
					NONE,
					PLAYER,
					ENEMY,
					};

				Field(): owner(Owner::NONE), turret(nullptr), collectibles() {}

				Owner owner;

				Turret* turret;
				std::vector<void*> collectibles;
				};

			// field[Y][X]
			std::vector<std::vector<Field*>> field;

			Engine::Graphics::ImagePtr fieldSprite;

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

			Engine::Math::Vector getFieldPosition(unsigned x, unsigned y) const;
			bool getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, unsigned& x, unsigned& y);
		};

	} /* namespace Game */
