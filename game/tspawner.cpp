/*
 * tspawner.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tspawner.h"

#include "../engine/debug/log.h"

#include "level.h"
#include "particleemitter.h"
#include "rules.h"

using namespace Game;

const int WAVE_COUNT=50;
const TSpawner::WaveDef WAVE_DEFINITIONS[WAVE_COUNT]=
	{
		{ 1,  0,  1.00f, 2.00f,  5.0f, 0},
		{ 1,  0,  1.00f, 2.00f,  5.0f, 0},
		{ 1,  0,  1.00f, 2.00f,  5.0f, 0},
		{ 1,  0,  1.00f, 2.00f,  5.0f, 0},
		{ 2,  0,  1.00f, 2.00f,  5.0f, 0},
		{ 2,  0,  1.00f, 2.00f,  4.5f, 0},
		{ 2,  0,  1.00f, 2.00f,  4.5f, 0},
		{ 3,  0,  1.00f, 2.00f,  4.5f, 0},
		{ 3,  0,  1.00f, 2.00f,  4.5f, 0},
		{ 4,  0,  1.00f, 1.75f,  4.0f, 0},
		{ 4,  0,  1.00f, 1.75f,  4.0f, 0},
		{ 5,  0,  1.00f, 1.75f,  4.0f, 0},
		{ 5,  0,  1.00f, 1.75f,  4.0f, 0},
		{ 5,  0,  1.00f, 1.75f,  3.5f, 0},
		{ 5,  0,  1.00f, 1.75f,  3.5f, 0},
		{ 4,  1,  1.00f, 1.75f, 10.0f, 1},
		{ 4,  1,  1.00f, 1.75f,  3.5f, 1},
		{ 4,  1,  1.00f, 1.75f,  3.0f, 1},
		{ 3,  2,  1.00f, 1.75f,  3.0f, 1},
		{ 3,  2,  1.00f, 1.50f,  3.0f, 1},
		{ 2,  3,  1.10f, 1.50f,  3.0f, 1},
		{ 2,  3,  1.21f, 1.50f,  3.0f, 1},
		{ 3,  3,  1.33f, 1.50f,  3.0f, 1},
		{ 3,  3,  1.46f, 1.50f,  3.0f, 1},
		{ 3,  3,  1.61f, 1.50f,  3.0f, 1},
		{ 4,  3,  1.77f, 1.50f,  3.0f, 1},
		{ 4,  3,  1.94f, 1.50f,  3.0f, 1},
		{ 4,  3,  2.14f, 1.50f,  3.0f, 1},
		{ 4,  3,  2.35f, 1.50f,  3.0f, 1},
		{ 5,  3,  2.59f, 1.50f,  3.0f, 1},
		{ 5,  3,  2.85f, 1.25f,  3.0f, 1},
		{ 5,  3,  3.13f, 1.25f,  3.0f, 1},
		{ 5,  3,  3.45f, 1.25f,  3.0f, 1},
		{ 4,  3,  3.79f, 1.25f,  3.0f, 1},
		{ 5,  2,  4.17f, 1.25f,  3.0f, 1},
		{ 3,  2,  4.59f, 1.00f,  3.0f, 1},
		{ 3,  1,  5.05f, 1.00f,  3.0f, 1},
		{ 2,  1,  5.55f, 1.00f,  3.0f, 1},
		{ 1,  0,  6.11f, 1.00f,  3.0f, 1},
		{ 0,  0,  6.72f, 1.00f, 10.0f, 2},
		{ 1,  1,  7.06f, 0.75f,  3.0f, 2},
		{ 2,  2,  7.41f, 0.75f,  3.0f, 2},
		{ 2,  3,  7.78f, 0.75f,  3.0f, 2},
		{ 3,  4,  8.17f, 0.75f,  3.0f, 2},
		{ 3,  5,  8.58f, 0.75f,  3.0f, 2},
		{ 4,  6,  9.01f, 0.70f,  3.0f, 2},
		{ 3,  7,  9.46f, 0.60f,  3.0f, 2},
		{ 2,  8,  9.93f, 0.55f,  3.0f, 2},
		{ 1,  9, 10.43f, 0.52f,  3.0f, 2},
		{ 0, 10, 10.95f, 0.50f,  3.0f, 2},
	};

bool TSpawner::init()
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/turret_spawner.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	/*if(!(spriteEnemy=Engine::Graphics::SpritePtr("sprite/unit_enemy_infantry.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}*/

	state=STATE_PRE_SPAWN;

	return true;
	}

bool TSpawner::updateFieldOwners() const
	{
	using namespace Engine::Math;

	if(!level->fieldClaim(fposition, Level::Field::Owner::ENEMY))
		{
		LOG_ERROR("Nie udalo sie ustawic wlasciciela pola %d,%d", fposition.x, fposition.y);
		return false;
		}

	level->fieldClaim(fposition+VectorI(-1,  0), Level::Field::Owner::ENEMY);
	level->fieldClaim(fposition+VectorI( 1,  0), Level::Field::Owner::ENEMY);
	level->fieldClaim(fposition+VectorI( 0, -1), Level::Field::Owner::ENEMY);
	level->fieldClaim(fposition+VectorI( 0,  1), Level::Field::Owner::ENEMY);

	return true;
	}

bool TSpawner::attachToLevel(Level* level, const Engine::Math::VectorI& fposition)
	{
	if(!Turret::init(level, fposition))
		{
		LOG_ERROR("Nie udalo sie zainicjowac wiezy");
		return false;
		}

	return true;
	}

bool TSpawner::removeFromLevel()
	{
	using namespace Engine::Math;

	level->fieldRelease(fposition, Level::Field::Owner::ENEMY);
	level->fieldRelease(fposition+VectorI(-1,  0), Level::Field::Owner::ENEMY);
	level->fieldRelease(fposition+VectorI( 1,  0), Level::Field::Owner::ENEMY);
	level->fieldRelease(fposition+VectorI( 0, -1), Level::Field::Owner::ENEMY);
	level->fieldRelease(fposition+VectorI( 0,  1), Level::Field::Owner::ENEMY);

	return true;
	}



void TSpawner::initStateSpawning()
	{
	state=STATE_SPAWNING;

	wave=0;
	waveUnit=0;
	waveCurDef=nullptr;

	cooldown=15.0f;

	level->addEmitter(new ParticleEmitter(ParticleEmitterType::EXPLOSION, level->getFieldPosition(fposition), cooldown, Engine::Graphics::SpritePtr("sprite/particle_spawn.xml"), 0.6f, 4.0f, 96.0f, 256.0f, 1.0f));
	}

void TSpawner::initStateNormal()
	{
	state=STATE_NORMAL;

	wave=0;
	waveCurDef=&WAVE_DEFINITIONS[wave];

	cooldown=waveCurDef->cooldownWave;
	waveUnit=0;
	}

void TSpawner::initStateAttacked()
	{
	//
	}

void TSpawner::initStateOverload()
	{
	/*state=STATE_OVERLOAD;

	wave=0;
	waveCurDef=&WAVE_DEFINITIONS[wave];

	cooldown=waveCurDef->cooldownWave;
	waveUnit=0;*/
	}

void TSpawner::update(float dt)
	{
	sprite.update(dt);

	cooldown-=dt;

	switch(state)
		{
		case STATE_PRE_SPAWN:
			initStateSpawning();
		break;

		case STATE_SPAWNING:
			updateStateSpawning(dt);
		break;

		case STATE_ATTACKED:
			updateStateAttacked(dt);
		break;

		case STATE_OVERLOAD:
			updateStateOverload(dt);
		break;

		default:
		case STATE_NORMAL:
			updateStateNormal(dt);
		break;
		}
	}

void TSpawner::updateStateSpawning(float dt)
	{
	if(cooldown<=0.0f)
		{
		initStateNormal();
		return;
		}
	}

void TSpawner::updateStateNormal(float dt)
	{
	using namespace Engine::Math;

	//LOG_DEBUG("[wave %d][cd %.2f/%.2f]", wave, cooldown, waveCurDef->cooldownWave);

	if(cooldown<=0.0f)
		{
		// Jeśli w poblizu sa wieze gracza - wejdz w tryb atakowanego
		for(int oy=-1; oy<=1; ++oy)
			{
			for(int ox=-1; ox<=1; ++ox)
				{
				const Level::Field* field=((const Level*)level)->getField(fposition+VectorI(ox, oy));

				if(!field || !field->turret)
					continue;

				if(field->turret->getType()==TurretType::PLAYER_UNIT_SINGLE_TARGET ||
				   field->turret->getType()==TurretType::PLAYER_UNIT_AREA_OF_EFFECT)
					{
					initStateAttacked();
					//return;
					}
				}
			}

		if(!isRuleEnabled(RULE_ENEMY_SPAWN_UNITS))
			{
			cooldown=1.0f;
			}
		else if(waveUnit>=waveCurDef->uinfantry+waveCurDef->uarmored)
			{
			if(wave<WAVE_COUNT-1)
				{
				++wave;
				}
			else
				{
				initStateOverload();
				//return;
				}

			waveCurDef=&WAVE_DEFINITIONS[wave];

			cooldown=waveCurDef->cooldownWave;
			waveUnit=0;

			sprite.setAnimation(waveCurDef->level);
			LOG_DEBUG("[wave %d][cd %.2f][i %2d a %2d cd %.2f][hp %.2f]", wave, waveCurDef->cooldownWave, waveCurDef->uinfantry, waveCurDef->uarmored, waveCurDef->cooldownUnit, waveCurDef->hp);
			}
		else
			{
			++waveUnit;
			cooldown=waveCurDef->cooldownUnit;

			if(waveUnit<waveCurDef->uarmored)
				level->spawnUnit(UnitType::ENEMY_ARMORED, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp, level->getFieldDiagonalSize()*0.5f);
			else
				level->spawnUnit(UnitType::ENEMY_INFANTRY, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp, level->getFieldDiagonalSize()*(0.5f + wave/20*0.2f));
			}
		}
	}

void TSpawner::updateStateAttacked(float dt)
	{
	hp=0.0f;
	}

void TSpawner::updateStateOverload(float dt)
	{
	if(cooldown<=0.0f)
		{

		}
	}
