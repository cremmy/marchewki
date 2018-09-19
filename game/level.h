/*
 * level.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include <list>
#include <vector>

#include "../engine/graphics/imageptr.h"
#include "../engine/graphics/spriteptr.h"

#include "turret_type.h"
#include "unit_type.h"

namespace Game
	{
	class Collectible;
	class ParticleEmitter;
	class Projectile;
	class Turret;
	class Unit;

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

				Field(): owner(Owner::NONE), ownerCount(0), highlight(false), turret(nullptr) {}

				Owner owner;
				int ownerCount;
				bool highlight;

				Turret* turret;
				};

		protected:
			struct GraphNode // out od date
				{
				enum
					{
					NEIGHBOUR_UP=0,
					NEIGHBOUR_LEFT=1,
					NEIGHBOUR_DOWN=2,
					NEIGHBOUR_RIGHT=3,
					};

				int x, y;
				float distance;
				GraphNode *prev;
				};

			Field* getField(const Engine::Math::VectorI& fposition);
			void updateFieldOwners();

			// field[Y][X]
			std::vector<std::vector<Field*>> field;
			std::vector<std::vector<GraphNode>> nodes;
			std::list<Unit*> units;
			std::list<Projectile*> projectiles;
			std::list<Collectible*> collectibles;
			std::list<ParticleEmitter*> emitters;

			Engine::Graphics::ImagePtr spriteFieldNeutral;
			Engine::Graphics::ImagePtr spriteFieldPlayer;
			Engine::Graphics::ImagePtr spriteFieldEnemy;

			unsigned ownedByPlayer;
			unsigned ownedByEnemy;
			unsigned turretsPlayer;
			unsigned turretsEnemy;
			unsigned farmsPlayer;

			float turretEnemySpawnCooldown;

			unsigned unlockedCollectibles;
			float resources;

			float highlightEmitterTimeout;

			unsigned pathVersion; // Zwiększane o 1 za każdym razem gdy ścieżki są przeliczane

		public:
			Level(): ownedByPlayer(0u), ownedByEnemy(0u), turretsPlayer(0u), turretsEnemy(0u), farmsPlayer(0u), turretEnemySpawnCooldown(0.0f),
				unlockedCollectibles(0u), resources(0.0f), highlightEmitterTimeout(0.0f), pathVersion(0u)
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
			Engine::Math::VectorI findPath(const Engine::Math::VectorI& from);
			Engine::Math::Vector findPath(const Engine::Math::Vector& from);
			void findPath(std::list<Engine::Math::VectorI>& path, const Engine::Math::Vector& from);

			bool isUnitOnField(const Engine::Math::VectorI& fposition);
			int getUnitsOnFieldCount(const Engine::Math::VectorI& fposition);
			int getUnitsOnFieldCount(const Engine::Math::VectorI& fposition, int& countEnemy, int& countPlayer);
			bool findUnitsOnField(const Engine::Math::VectorI& fposition, std::vector<Unit*>& matches);
			Unit* findUnitInRange(const Engine::Math::Vector& position, float range, bool (*compare)(Unit*, Unit*));
			bool findAllUnitsInRange(const Engine::Math::Vector& position, float range, std::vector<Unit*>& matches);
			Collectible* findUnlockedCollectible();

			bool isDepleted() const {return resources<=0.0f;}
			float getResources() const {return resources;}
			float getResourceDrain(bool gainOnly=false) const;
			float getResourceDrainFields() const;
			const Field* getField(const Engine::Math::VectorI& fposition) const;
			Field::Owner getFieldOwner(const Engine::Math::VectorI& fposition) const;
			unsigned getFieldWidth() const {return spriteFieldNeutral->getW();}
			unsigned getFieldHeight() const {return spriteFieldNeutral->getH();}
			float getFieldDiagonalSize() const {return sqrt(getFieldWidth()*getFieldWidth() + getFieldHeight()*getFieldHeight());}
			int getWidth() const {return field.empty()?0:field[0u].size();}
			int getHeight() const {return field.size();}
			unsigned getPlayerFieldCount() const {return ownedByPlayer;}
			unsigned getEnemyFieldCount() const {return ownedByEnemy;}
			unsigned getPlayerTurretCount() const {return turretsPlayer;}
			unsigned getEnemyTurretCount() const {return turretsEnemy;}
			unsigned getPlayerFarmCount() const {return farmsPlayer;}
			unsigned getPathVersion() const {return pathVersion;}
			unsigned getUnlockedCollectiblesCount() {return unlockedCollectibles;}

			Engine::Math::Vector getFieldPosition(const Engine::Math::VectorI& fposition) const;
			Engine::Math::VectorI getPositionOnField(const Engine::Math::Vector& position) const;
			Field* getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction);
			Field* getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, Engine::Math::VectorI& fposition);

			void setResources(float s) {resources=s;}
			void addResources(float s) {resources+=s; if(resources<0.0f) resources=0.0f;}
			bool fieldClaim(const Engine::Math::VectorI& fposition, Field::Owner who);// {return (getFieldOwner(fposition)==Field::Owner::NONE)?setFieldOwner(fposition, who):(getFieldOwner(fposition)==who);}
			bool fieldRelease(const Engine::Math::VectorI& fposition, Field::Owner who);// {return (getFieldOwner(fposition)==who)?setFieldOwner(fposition, Field::Owner::NONE):false;}
			bool setFieldOwner(const Engine::Math::VectorI& fposition, Field::Owner owner);
			bool setFieldHighlight(const Engine::Math::VectorI& fposition);
			bool buildTurret(const Engine::Math::VectorI& fposition, TurretType type);
			bool destroyTurret(const Engine::Math::VectorI& fposition, bool noCost=false);

			bool spawnUnit(UnitType type, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target, float hp, float speed);
			bool spawnCollectible(const Engine::Math::Vector& position, float value);

			bool addProjectile(Projectile* projectile);
			bool addEmitter(ParticleEmitter* emitter);
		};

	} /* namespace Game */
