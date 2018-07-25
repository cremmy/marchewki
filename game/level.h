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

#include "turret_type.h"

namespace Game
	{
	class Turret;

	class Level
		{
		public:
			struct Field
				{
				enum class Owner
					{
					NONE,
					PLAYER,
					ENEMY,
					};

				Field(): owner(Owner::NONE), turret(nullptr) {}

				Owner owner;

				Turret* turret;
				};

		protected:
			Field* getField(unsigned x, unsigned y);
			void updateFieldOwners();

			// field[Y][X]
			std::vector<std::vector<Field*>> field;

			Engine::Graphics::ImagePtr fieldSprite;

			unsigned ownedByPlayer;
			unsigned ownedByEnemy;
			unsigned turretsPlayer;
			unsigned turretsEnemy;

		public:
			Level(): ownedByPlayer(0u), ownedByEnemy(0u), turretsPlayer(0u), turretsEnemy(0u)
				{
				//
				}
			virtual ~Level()
				{
				clear();
				}

			bool init(unsigned w, unsigned h);
			void update(float dt);
			void print(float tinterp);
			void clear();

			bool resize(unsigned w, unsigned h);
			bool resizeIncreaseXByOne();
			bool resizeIncreaseYByOne();

			bool refreshPath();
			Engine::Math::VectorI FindPath(int x, int y);

			const Field* getField(unsigned x, unsigned y) const;
			Field::Owner getFieldOwner(unsigned x, unsigned y) const;
			unsigned getWidth() const {return field.empty()?0u:field[0u].size();}
			unsigned getHeight() const {return field.size();}
			unsigned getPlayerFieldCount() const {return ownedByPlayer;}
			unsigned getEnemyFieldCount() const {return ownedByEnemy;}
			unsigned getPlayerTurretCount() const {return turretsPlayer;}
			unsigned getEnemyTurretCount() const {return turretsEnemy;}

			Engine::Math::Vector getFieldPosition(unsigned x, unsigned y) const;
			Field* getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction);
			Field* getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, unsigned& x, unsigned& y);

			bool setFieldOwner(unsigned x, unsigned y, Field::Owner owner);
			bool buildTurret(unsigned x, unsigned y, TurretType type);
			bool destroyTurret(unsigned x, unsigned y);
		};

	} /* namespace Game */
