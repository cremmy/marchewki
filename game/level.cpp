/*
 * level.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "level.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "collectible.h"

#include "projectile.h"

#include "turret.h"
#include "tareaofeffect.h"
#include "tcarrotfield.h"
#include "tmine.h"
#include "tplayerbase.h"
#include "tsingletarget.h"
#include "tspawner.h"

#include "unit.h"
#include "uenemyinfantry.h"
#include "uplayeracolyte.h"

using namespace Game;

bool Level::init(unsigned w, unsigned h)
	{
	clear();

	if(!(spriteFieldNeutral=Engine::Graphics::ImagePtr("image/tile_neutral.png")))
		{
		return false;
		}
	if(!(spriteFieldPlayer=Engine::Graphics::ImagePtr("image/tile_player_turret.png")))
		{
		return false;
		}
	if(!(spriteFieldEnemy=Engine::Graphics::ImagePtr("image/tile_enemy.png")))
		{
		return false;
		}

	return resize(w, h);
	}

void Level::update(float dt)
	{
	const int W=getWidth();
	const int H=getHeight();

	for(int y=0; y<H; ++y)
		{
		for(int x=0; x<W; ++x)
			{
			Field* field=getField({x, y});

			if(field->turret)
				{
				field->turret->update(dt);
				}
			}
		}

	//for(auto unit: units)
	for(auto it=units.begin(); it!=units.end(); ++it)
		{
		Unit* unit=*it;

		if(!unit->isAlive() && !unit->isLocked())
			{
			units.erase(it--);
			delete unit;
			continue;
			}

		unit->update(dt);
		}

	for(auto it=projectiles.begin(); it!=projectiles.end(); ++it)
		{
		Projectile* projectile=*it;

		if(!projectile->isAlive())
			{
			projectiles.erase(it--);
			delete projectile;
			continue;
			}

		projectile->update(dt);
		}

	for(auto it=collectibles.begin(); it!=collectibles.end(); ++it)
		{
		Collectible* collectible=*it;

		if(!collectible->isAlive())
			{
			collectibles.erase(it--);
			delete collectible;
			continue;
			}

		collectible->update(dt);
		}
	}

void Level::print(float tinterp)
	{
	using namespace Engine::Math;

	const int W=getWidth();
	const int H=getHeight();

	glDisable(GL_DEPTH_TEST);
	for(int y=0u; y<H; ++y)
		{
		for(int x=0u; x<W; ++x)
			{
			Field* field=getField({x, y});

			const Vector position=Engine::Math::Vector(
					(x  )*getFieldWidth(),
					(y+1)*getFieldHeight());

			/*switch(field->owner)
				{
				case Field::Owner::PLAYER:
					Engine::Render::getInstance().setColor(Vector(0.7f, 1.0f, 0.7f, 1.0f));
				break;

				case Field::Owner::ENEMY:
					Engine::Render::getInstance().setColor(Vector(1.0f, 0.7f, 0.7f, 1.0f));
				break;

				default:
				case Field::Owner::NONE:
					Engine::Render::getInstance().setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				}*/

			if(field->owner==Field::Owner::ENEMY)
				Engine::Render::getInstance().draw(Orientation::FLAT_XY+position, spriteFieldEnemy);
			else if(field->owner==Field::Owner::PLAYER && !(field->turret && field->turret->getType()==TurretType::PLAYER_CARROT_FIELD))
				Engine::Render::getInstance().draw(Orientation::FLAT_XY+position, spriteFieldPlayer);
			else
				Engine::Render::getInstance().draw(Orientation::FLAT_XY+position, spriteFieldNeutral);

			/*const GraphNode& node=nodes[y][x];
			if(node.prev)
				{
				const Vector center=getFieldPosition({x, y});
				const Vector dir=VectorNormalize(getFieldPosition({node.prev->x, node.prev->y}) - center);
				const Vector head=center+dir*32.0f;
				const Vector headL=MatrixRotZ<float>(135.0f)*dir*16.0f;
				const Vector headR=MatrixRotZ<float>(-135.0f)*dir*16.0f;

				Engine::Render::getInstance().drawLine(center+Vector(0, 0, 8), head+Vector(0, 0, 8), Vector(1.0f, 1.0f, 1.0f, 0.75f));
				Engine::Render::getInstance().drawLine(head+Vector(0, 0, 8), head+headL+Vector(0, 0, 8), Vector(1.0f, 1.0f, 1.0f, 0.75f));
				Engine::Render::getInstance().drawLine(head+Vector(0, 0, 8), head+headR+Vector(0, 0, 8), Vector(1.0f, 1.0f, 1.0f, 0.75f));
				}*/
			}
		}

	// Płaskie działka
	Engine::Render::getInstance().setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));
	for(int y=0; y<H; ++y)
		{
		for(int x=0; x<W; ++x)
			{
			Field* field=getField({x, y});

			if(field->turret && field->turret->isFlat())
				{
				field->turret->print(tinterp);
				}
			}
		}

	glEnable(GL_DEPTH_TEST);

	// Wypukłe działka
	//Engine::Render::getInstance().setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));
	for(int y=0; y<H; ++y)
		{
		for(int x=0; x<W; ++x)
			{
			Field* field=getField({x, y});

			if(field->turret && !field->turret->isFlat())
				{
				field->turret->print(tinterp);
				}
			}
		}

	for(auto unit: units)
		{
		unit->print(tinterp);
		}

	for(auto projectile: projectiles)
		{
		projectile->print(tinterp);
		}

	for(auto collectible: collectibles)
		{
		collectible->print(tinterp);
		}
	}

void Level::clear()
	{
	for(unsigned y=0u; y<field.size(); ++y)
		{
		for(unsigned x=0u; x<field[y].size(); ++x)
			{
			if(field[y][x]->turret)
				{
				delete field[y][x]->turret;
				}

			delete field[y][x];
			}

		field[y].clear();
		}

	field.clear();

	for(unsigned y=0u; y<nodes.size(); ++y)
		{
		nodes[y].clear();
		}
	nodes.clear();

	for(auto& unit: units)
		{
		delete unit;
		}
	units.clear();

	for(auto& projectile: projectiles)
		{
		delete projectile;
		}
	projectiles.clear();

	for(auto& collectible: collectibles)
		{
		delete collectible;
		}
	collectibles.clear();

	spriteFieldNeutral=nullptr;
	spriteFieldPlayer=nullptr;
	spriteFieldEnemy=nullptr;
	}


bool Level::resize(unsigned w, unsigned h)
	{
	const unsigned OLD_H=field.size();
	const unsigned OLD_W=field.empty()?0u:field[0u].size();

	// Pomniejszanie, skasuj nadmiarowe pola
	if(h<OLD_H)
		{
		for(unsigned y=h; y<OLD_H; ++y)
			{
			for(unsigned x=0u; x<OLD_W; ++x)
				{
				delete field[y][x];
				}
			field[y].clear();
			}
		}

	field.resize(h);

	if(h>OLD_H)
		{
		for(unsigned y=OLD_H; y<h; ++y)
			{
			field[y].resize(w);

			for(unsigned x=0u; x<OLD_W; ++x)
				{
				field[y][x]=new Field();
				}
			}
		}

	if(w<OLD_W)
		{
		for(unsigned y=0; y<h; ++y)
			{
			// Tutaj już musi zostać field[y].size(), bo przy powiększaniu w H tworzone są tablice odpowiedniej wielkości
			for(unsigned x=w; x<field[y].size(); ++x)
				{
				delete field[y][x];
				}

			field[y].resize(w);
			}
		}
	else if(w>OLD_W)
		{
		for(unsigned y=0; y<h; ++y)
			{
			for(unsigned x=OLD_W; x<field[y].size(); ++x)
				{
				field[y][x]=new Field();
				}
			}
		}

	return true;
	}

bool Level::resizeIncreaseXByOne()
	{
	return resize(field.empty()?1u:field[0u].size()+1u, field.size());
	}

bool Level::resizeIncreaseYByOne()
	{
	return resize(field.empty()?1u:field[0u].size(), field.size()+1u);
	}


bool Level::isUnitOnField(const Engine::Math::VectorI& fposition)
	{
	using namespace Engine::Math;

	const Vector fRealPosition=getFieldPosition(fposition);
	const float MAX_DISTANCE=sqrt(getFieldWidth()*getFieldWidth() + getFieldHeight()*getFieldHeight());

	for(auto unit: units)
		{
		if(VectorLength(unit->getPosition()-fRealPosition)>MAX_DISTANCE)
			continue;

		const VectorI fPositionUnit=getPositionOnField(unit->getPosition());

		if(fPositionUnit.x!=fposition.x || fPositionUnit.y!=fposition.y)
			continue;

		return true;
		}

	return false;
	}

Unit* Level::findUnitInRange(const Engine::Math::Vector& position, float range, bool (*compare)(Unit*, Unit*))
	{
	using namespace Engine::Math;

	if(units.empty())
		{
		return nullptr;
		}

	Unit* found=nullptr;

	for(auto unit: units)
		{
		if(!unit->isAlive() ||
		   unit->getType()==UnitType::PLAYER_ACOLYTE ||
		   VectorLength(unit->getPosition()-position)>range ||
		   (found && !compare(unit, found)))
			{
			continue;
			}

		found=unit;
		}

	return found;
	}

bool Level::findAllUnitsInRange(const Engine::Math::Vector& position, float range, std::vector<Unit*>& matches)
	{
	using namespace Engine::Math;

	matches.clear();

	if(units.empty())
		{
		return false;
		}

	for(auto unit: units)
		{
		if(!unit->isAlive() ||
		   unit->getType()==UnitType::PLAYER_ACOLYTE ||
		   VectorLength(unit->getPosition()-position)>range)
			{
			continue;
			}

		matches.push_back(unit);
		}

	return !matches.empty();
	}

Collectible* Level::findUnlockedCollectible()
	{
	if(unlockedCollectibles==0u)
		{
		return nullptr;
		}

	for(auto collectible: collectibles)
		{
		if(collectible->isLocked())
			continue;

		--unlockedCollectibles;
		collectible->lock();
		return collectible;
		}

	return nullptr;
	}


Level::Field* Level::getField(const Engine::Math::VectorI& fposition)
	{
	if(fposition.x<0 || fposition.y<0 || fposition.x>=getWidth() || fposition.y>=getHeight())
		return nullptr;
	return field[fposition.y][fposition.x];
	}

const Level::Field* Level::getField(const Engine::Math::VectorI& fposition) const
	{
	if(fposition.x<0 || fposition.y<0 || fposition.x>=getWidth() || fposition.y>=getHeight())
		return nullptr;
	return field[fposition.y][fposition.x];
	}

Level::Field::Owner Level::getFieldOwner(const Engine::Math::VectorI& fposition) const
	{
	if(const Field* field=getField(fposition))
		{
		return field->owner;
		}

	return Field::Owner::NONE;
	}

Engine::Math::Vector Level::getFieldPosition(const Engine::Math::VectorI& fposition) const
	{
	return Engine::Math::Vector(
		fposition.x*getFieldWidth()+getFieldWidth()*0.5,
		fposition.y*getFieldHeight()+getFieldHeight()*0.5);
	}

Engine::Math::VectorI Level::getPositionOnField(const Engine::Math::Vector& position) const
	{
	// Odwazne zalozenie: postacie nigdy nie wyjda poza poziom
	// Potencjalne problemy: co jeśli plansza się zmniejszy i ucieknie im spod nóg?
	// Proponowane rozwiązanie: Cóż.

	return Engine::Math::VectorI(
		floor(position.x/getFieldWidth()),
		floor(position.y/getFieldHeight()));
	}

Level::Field* Level::getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction)
	{
	Engine::Math::VectorI fposition;

	return getFieldByRay(position, direction, fposition);
	}

Level::Field* Level::getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, Engine::Math::VectorI& fposition)
	{
	//LOG_DEBUG(LOG_STR_VECTOR(position));
	//LOG_DEBUG(LOG_STR_VECTOR(direction));

	using namespace Engine::Math;

	const float Z_DISTANCE=position.z/direction.z;
	const Vector HIT=position-direction*Z_DISTANCE;

	if(HIT.x<0 || HIT.y<0)
		{
		return nullptr;
		}

	const int HIT_X=HIT.x/getFieldWidth();
	const int HIT_Y=HIT.y/getFieldHeight();

	//LOG_DEBUG("Dist: %f; Hit: %f, %f, %f; %d %d", Z_DISTANCE, HIT.x, HIT.y, HIT.z, HIT_X, HIT_Y);

	if(/*HIT_X<0 ||*/ HIT_X>=getWidth() ||
	   /*HIT_Y<0 ||*/ HIT_Y>=getHeight())
		{
		return nullptr;
		}

	fposition.x=HIT_X;
	fposition.y=HIT_Y;

	return getField(fposition);
	}


bool Level::setFieldOwner(const Engine::Math::VectorI& fposition, Field::Owner owner)
	{
	Field* field=getField(fposition);

	if(!field)
		return false;

	if(field->owner==owner)
		{
		return true;
		}

	switch(field->owner)
		{
		case Field::Owner::PLAYER:
			if(owner==Field::Owner::ENEMY)
				{
				return false;
				}

			--ownedByPlayer;
		break;

		case Field::Owner::ENEMY:
			if(owner==Field::Owner::PLAYER)
				{
				return false;
				}

			--ownedByEnemy;
		break;

		default:
		case Field::Owner::NONE:
			//
		break;
		}

	field->owner=owner;

	switch(field->owner)
		{
		case Field::Owner::PLAYER:
			++ownedByPlayer;
		break;

		case Field::Owner::ENEMY:
			++ownedByEnemy;
		break;

		default:
		case Field::Owner::NONE:
			//
		break;
		}

	return true;
	}

bool Level::buildTurret(const Engine::Math::VectorI& fposition, TurretType type)
	{
	Field* field=getField(fposition);

	if(!field)
		return false;

	if(field->turret)
		{
		LOG_WARNING("Pole %d,%d jest juz zajete", fposition.x, fposition.y);
		return false;
		}

	if(isUnitOnField(fposition))
		{
		{
		LOG_WARNING("Jednostki stoja na polu %d,%d", fposition.x, fposition.y);
		return false;
		}
		}

	switch(type)
		{
		case TurretType::PLAYER_BASE:
		case TurretType::PLAYER_CARROT_FIELD:
		case TurretType::PLAYER_UNIT_SINGLE_TARGET:
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT:
		case TurretType::PLAYER_UNIT_MINE:
			if(field->owner==Field::Owner::ENEMY)
				{
				LOG_WARNING("Nie mozna zbudowac wiezy gracza na polu przeciwnika");
				return false;
				}
		break;

		case TurretType::ENEMY_SPAWNER:
			if(field->owner==Field::Owner::PLAYER)
				{
				LOG_WARNING("Nie mozna zbudowac wiezy przeciwnika na polu gracza");
				return false;
				}
		break;

		default:
			//
		break;
		}

	// TODO Sprawdzić czy na polu nie ma jednostek

	Turret* turret=nullptr;

	switch(type)
		{
		case TurretType::PLAYER_BASE:
			turret=new TPlayerBase();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_CARROT_FIELD:
			turret=new TCarrotField();
			++turretsPlayer;
			++farmsPlayer;
		break;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:
			turret=new TSingleTarget();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT:
			turret=new TAreaOfEffect();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_UNIT_MINE:
			turret=new TMine();
			++turretsPlayer;
		break;

		case TurretType::ENEMY_SPAWNER:
			turret=new TSpawner();
			++turretsEnemy;
		break;

		default:
			//
		break;
		}

	if(!turret)
		{
		LOG_ERROR("Nie udalo sie utworzyc wiezy");
		return false;
		}

	field->turret=turret;

	if(!turret->init())
		{
		LOG_WARNING("Nie udalo sie zainicjowac wiezy");
		return destroyTurret(fposition);
		}
	if(!turret->attachToLevel(this, fposition))
		{
		LOG_WARNING("Nie udalo sie umiescic wiezy w poziomie");
		return destroyTurret(fposition);
		}

	if(!refreshPath())
		{
		LOG_WARNING("Nie mozna umiescic wiezy w %d,%d", fposition.x, fposition.y);

		if(!destroyTurret(fposition))
			{
			LOG_ERROR("...i nie udalo sie tez jej skasowac, AAAAaaaaa");
			}

		return false;
		}

	updateFieldOwners();

	return true;
	}

bool Level::destroyTurret(const Engine::Math::VectorI& fposition)
	{
	Field* field=getField(fposition);

	if(!field)
		return false;

	if(!field->turret)
		return false;

	switch(field->turret->getType())
		{
		case TurretType::PLAYER_CARROT_FIELD:
			if(farmsPlayer>0u)
				{
				--farmsPlayer;
				}
				/* no break */
		// break;

		case TurretType::PLAYER_BASE:
		case TurretType::PLAYER_UNIT_SINGLE_TARGET:
		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT:
		case TurretType::PLAYER_UNIT_MINE:
			if(turretsPlayer==0)
				{
				LOG_WARNING("Licznik wiez gracza zszedl ponizej zera");
				break;
				}

			--turretsPlayer;
		break;

		case TurretType::ENEMY_SPAWNER:
			if(turretsEnemy==0)
				{
				LOG_WARNING("Licznik wiez przeciwnika zszedl ponizej zera");
				break;
				}

			--turretsEnemy;
		break;

		default:
			//
		break;
		}

	field->turret->removeFromLevel();
	delete field->turret;
	field->turret=nullptr;

	updateFieldOwners();

	refreshPath();

	return true;
	}

void Level::updateFieldOwners()
	{
	const int W=getWidth();
	const int H=getHeight();

	for(int y=0; y<H; ++y)
		{
		for(int x=0; x<W; ++x)
			{
			Field* field=getField({x, y});

			if(!field->turret)
				continue;

			field->turret->updateFieldOwners();
			}
		}
	}

bool Level::spawnUnit(UnitType type, const Engine::Math::VectorI& fposition, const Engine::Math::VectorI& target, float hp, float speed)
	{
	using namespace Engine::Math;

	Field* field=getField(fposition);

	if(!field)
		return false;

	if(field->turret && !field->turret->isWalkable())
		{
		LOG_WARNING("Pole %d,%d jest juz zajete i nie mozna po nim chodzic", fposition.x, fposition.y);
		return false;
		}

	Unit* unit=nullptr;

	switch(type)
		{
		case UnitType::PLAYER_ACOLYTE:
			unit=new UPlayerAcolyte();
		break;

		case UnitType::ENEMY_INFANTRY:
			unit=new UEnemyInfantry();
		break;

		case UnitType::ENEMY_ARMORED:
			//unit=new UEnemyInfantry();
		break;

		default:
			//
		break;
		}

	if(!unit)
		{
		LOG_ERROR("Nie udalo sie utworzyc jednostki");
		return false;
		}

	if(!unit->init(this, fposition, target))
		{
		LOG_WARNING("Nie udalo sie zainicjowac jednostki");
		delete unit;
		return false;
		}

	unit->setHP(hp);
	unit->setSpeed(speed);

	units.push_back(unit);

	return true;
	}

bool Level::spawnCollectible(const Engine::Math::Vector& position, float value)
	{
	using namespace Engine::Math;

	Collectible* collectible=new Collectible();

	if(!collectible)
		{
		LOG_ERROR("Nie udalo sie utworzyc zbierajki");
		return false;
		}

	if(!collectible->init(position, value))
		{
		LOG_WARNING("Nie udalo sie zainicjowac zbierajki");
		delete collectible;
		return false;
		}

	++unlockedCollectibles;
	collectibles.push_back(collectible);

	return true;
	}

bool Level::addProjectile(Projectile* projectile)
	{
	projectiles.push_back(projectile);
	return true;
	}
