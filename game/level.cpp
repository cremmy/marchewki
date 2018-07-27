/*
 * level.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "level.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "turret.h"
#include "tplayerbase.h"
#include "tspawner.h"
#include "unit.h"
#include "uenemyinfantry.h"

using namespace Game;

bool Level::init(unsigned w, unsigned h)
	{
	clear();

	if(!(fieldSprite=Engine::Graphics::ImagePtr("image/tile.png")))
		{
		return false;
		}

	return resize(w, h);
	}

void Level::update(float dt)
	{
	const unsigned W=getWidth();
	const unsigned H=getHeight();

	for(unsigned y=0u; y<H; ++y)
		{
		for(unsigned x=0u; x<W; ++x)
			{
			Field* field=getField(x, y);

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

		if(!unit->isAlive())
			{
			units.erase(it--);
			delete unit;
			continue;
			}

		unit->update(dt);
		}
	}

void Level::print(float tinterp)
	{
	using namespace Engine::Math;

	const unsigned W=getWidth();
	const unsigned H=getHeight();

	glDisable(GL_DEPTH_TEST);
	for(unsigned y=0u; y<H; ++y)
		{
		for(unsigned x=0u; x<W; ++x)
			{
			Field* field=getField(x, y);

			switch(field->owner)
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
				}

			Engine::Render::getInstance().draw(Orientation::FLAT_XY+Vector(x*fieldSprite->getW(), (y+1u)*fieldSprite->getH()), fieldSprite);
			}
		}
	glEnable(GL_DEPTH_TEST);

	Engine::Render::getInstance().setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));
	for(unsigned y=0u; y<H; ++y)
		{
		for(unsigned x=0u; x<W; ++x)
			{
			Field* field=getField(x, y);

			if(field->turret)
				{
				field->turret->print(tinterp);
				}
			}
		}

	for(auto unit: units)
		{
		unit->print(tinterp);
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

	fieldSprite=nullptr;
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


Level::Field* Level::getField(unsigned x, unsigned y)
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}

const Level::Field* Level::getField(unsigned x, unsigned y) const
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}

Level::Field::Owner Level::getFieldOwner(unsigned x, unsigned y) const
	{
	if(const Field* field=getField(x, y))
		{
		return field->owner;
		}

	return Field::Owner::NONE;
	}

Engine::Math::Vector Level::getFieldPosition(unsigned x, unsigned y) const
	{
	return Engine::Math::Vector(x*fieldSprite->getW()+fieldSprite->getW()*0.5, y*fieldSprite->getH()+fieldSprite->getH()*0.5);
	}

Engine::Math::VectorI Level::getPositionOnField(const Engine::Math::Vector& position) const
	{
	// Odwazne zalozenie: postacie nigdy nie wyjda poza poziom
	// Potencjalne problemy: co jeśli plansza się zmniejszy i ucieknie im spod nóg?
	// Proponowane rozwiązanie: Cóż.

	return Engine::Math::VectorI(
		floor(position.x/fieldSprite->getW()),
		floor(position.y/fieldSprite->getH()));
	}

Level::Field* Level::getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction)
	{
	unsigned x;
	unsigned y;

	return getFieldByRay(position, direction, x, y);
	}

Level::Field* Level::getFieldByRay(const Engine::Math::Vector& position, const Engine::Math::Vector& direction, unsigned& x, unsigned& y)
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

	const int HIT_X=HIT.x/fieldSprite->getW();
	const int HIT_Y=HIT.y/fieldSprite->getH();

	//LOG_DEBUG("Dist: %f; Hit: %f, %f, %f; %d %d", Z_DISTANCE, HIT.x, HIT.y, HIT.z, HIT_X, HIT_Y);

	if(/*HIT_X<0 ||*/ HIT_X>=(int)getWidth() ||
	   /*HIT_Y<0 ||*/ HIT_Y>=(int)getHeight())
		{
		return nullptr;
		}

	x=HIT_X;
	y=HIT_Y;

	return getField(x, y);
	}


bool Level::setFieldOwner(unsigned x, unsigned y, Field::Owner owner)
	{
	Field* field=getField(x, y);

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

bool Level::buildTurret(unsigned x, unsigned y, TurretType type)
	{
	Field* field=getField(x, y);

	if(!field)
		return false;

	if(field->turret)
		{
		LOG_WARNING("Pole %d,%d jest juz zajete", x, y);
		return false;
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
			//turret=new TPlayerBase();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_UNIT_SINGLE_TARGET:
			//turret=new TPlayerBase();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_UNIT_AREA_OF_EFFECT:
			//turret=new TPlayerBase();
			++turretsPlayer;
		break;

		case TurretType::PLAYER_UNIT_MINE:
			//turret=new TPlayerBase();
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
		return destroyTurret(x, y);
		}
	if(!turret->attachToLevel(this, x, y))
		{
		LOG_WARNING("Nie udalo sie umiescic wiezy w poziomie");
		return destroyTurret(x, y);
		}

	if(!refreshPath())
		{
		LOG_WARNING("Nie mozna umiescic wiezy w %d,%d", x, y);

		if(!destroyTurret(x, y))
			{
			LOG_ERROR("...i nie udalo sie tez jej skasowac, AAAAaaaaa");
			}

		return false;
		}

	updateFieldOwners();

	return true;
	}

bool Level::destroyTurret(unsigned x, unsigned y)
	{
	Field* field=getField(x, y);

	if(!field)
		return false;

	if(!field->turret)
		return false;

	switch(field->turret->getType())
		{
		case TurretType::PLAYER_BASE:
		case TurretType::PLAYER_CARROT_FIELD:
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
	const unsigned W=getWidth();
	const unsigned H=getHeight();

	for(unsigned y=0u; y<H; ++y)
		{
		for(unsigned x=0u; x<W; ++x)
			{
			Field* field=getField(x, y);

			if(!field->turret)
				continue;

			field->turret->updateFieldOwners();
			}
		}
	}

bool Level::spawnUnit(UnitType type, const Engine::Math::VectorI& position, const Engine::Math::VectorI& target, float hp, float speed)
	{
	using namespace Engine::Math;

	Field* field=getField(position.x, position.y);

	if(!field)
		return false;

	if(field->turret && !field->turret->isWalkable())
		{
		LOG_WARNING("Pole %d,%d jest juz zajete i nie mozna po nim chodzic", position.x, position.y);
		return false;
		}

	Unit* unit=nullptr;

	switch(type)
		{
		case UnitType::PLAYER_ACOLYTE:
			//unit=new UEnemyInfantry();
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

	if(!unit->init(this, position, target))
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
