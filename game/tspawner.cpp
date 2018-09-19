/*
 * tspawner.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "tspawner.h"

#include "../engine/debug/log.h"

#include "consts.h"
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
		{ 1,  0,  1.00f, 2.00f,  5.0f, 0},
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


bool TSpawner::isFieldSafe(const Level* level, const Engine::Math::VectorI& fposition)
	{
	using namespace Engine::Math;

	for(int oy=-1; oy<=1; ++oy)
		{
		for(int ox=-1; ox<=1; ++ox)
			{
			const Level::Field* field=level->getField(fposition+VectorI(ox, oy));

			if(!field)
				continue;

			if(!field->turret)
				continue;

			if(field->turret->getType()==TurretType::PLAYER_UNIT_AREA_OF_EFFECT)
				return false;
			}
		}

	return true;
	}

bool TSpawner::isFieldExtraSafe(const Level* level, const Engine::Math::VectorI& fposition)
	{
	using namespace Engine::Math;

	for(int oy=-1; oy<=1; ++oy)
		{
		for(int ox=-1; ox<=1; ++ox)
			{
			const Level::Field* field=level->getField(fposition+VectorI(ox, oy));

			if(!field)
				continue;

			if(field->owner==Level::Field::Owner::PLAYER)
				return false;
			}
		}

	return true;
	}


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

	cooldown=SPAWNER_COOLDOWN_SPAWNING;

	level->addEmitter(new ParticleEmitter(ParticleEmitterType::EXPLOSION, level->getFieldPosition(fposition), cooldown, Engine::Graphics::SpritePtr("sprite/particle_spawn.xml"), 0.6f, 4.0f, 96.0f, 256.0f, 1.0f));
	}

void TSpawner::initStateNormal()
	{
	state=STATE_NORMAL;

	wave=0;
	waveCurDef=&WAVE_DEFINITIONS[wave];

	cooldown=waveCurDef->cooldownWave;
	waveUnit=0;

	spreadCooldown=SPAWNER_TIMEOUT_SPREAD;
	}

void TSpawner::initStateSpreading()
	{
	if(!isRuleEnabled(RULE_ENEMY_BUILD_TURRETS))
		return;

	state=STATE_SPREADING;

	wave=0;
	waveCurDef=nullptr;

	cooldown=SPAWNER_COOLDOWN_SPREAD_PRE;
	waveUnit=0;
	}

void TSpawner::initStatePanic()
	{
	state=STATE_PANIC;

	//cooldown=waveCurDef->cooldownWave;
	}

void TSpawner::initStateOvercharge()
	{
	state=STATE_OVERCHARGE;

	cooldown=0.0f;
	wave=0;
	waveUnit=0;
	}

void TSpawner::update(float dt)
	{
	sprite.update(dt);

	if(hp<=0.0f)
		return;

	if(cooldown>0.0f)
		cooldown-=dt;

	switch(state)
		{
		case STATE_PRE_SPAWN:
			initStateSpawning();
		break;

		case STATE_SPAWNING:
			updateStateSpawning(dt);
		break;

		case STATE_SPREADING:
		case STATE_SPREADING_POST:
			updateStateSpreading(dt);
		break;

		case STATE_PANIC:
			updateStatePanic(dt);
		break;

		case STATE_OVERCHARGE:
			updateStateOvercharge(dt);
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

	if(spreadCooldown>0.0f)
		spreadCooldown-=dt;

	if(cooldown>0.0f)
		return;

	if(!isFieldSafe(level, fposition) || hp<0.5f)
		{
		return initStatePanic();
		}
	else if(isRuleEnabled(RULE_ENEMY_BUILD_TURRETS) && spreadCooldown<=0.0f)
		{
		if(canSpread())
			return initStateSpreading();

		spreadCooldown=SPAWNER_TIMEOUT_SPREAD;
		}
	else if(spreadCooldown<=0.0f && getNearbySpawnerCount(level, fposition)>4)
		{
		return initStateOvercharge();
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
			initStateSpreading();
			//return;
			}

		waveCurDef=&WAVE_DEFINITIONS[wave];

		cooldown=waveCurDef->cooldownWave;
		waveUnit=0;

		sprite.setAnimation(waveCurDef->level);
		//LOG_DEBUG("[wave %d][cd %.2f][i %2d a %2d cd %.2f][hp %.2f]", wave, waveCurDef->cooldownWave, waveCurDef->uinfantry, waveCurDef->uarmored, waveCurDef->cooldownUnit, waveCurDef->hp);
		}
	else
		{
		cooldown=waveCurDef->cooldownUnit;

		if(waveUnit<waveCurDef->uarmored)
			level->spawnUnit(UnitType::ENEMY_ARMORED, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp, level->getFieldDiagonalSize()*0.5f);
		else
			level->spawnUnit(UnitType::ENEMY_INFANTRY, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp, level->getFieldDiagonalSize()*(0.5f + wave/20*0.2f));

		++waveUnit;
		}
	}

void TSpawner::updateStateSpreading(float dt)
	{
	using namespace Engine::Math;

	if(state==STATE_SPREADING)
		{
		if(cooldown>0.0f)
			return;

		// Wybierz losowe pole w zasięgu
		if(!canSpread())
			{
			return initStateNormal();
			}

		bool good=false;
		for(int i=0; i<4*SPAWNER_SPREAD_DISTANCE*SPAWNER_SPREAD_DISTANCE; ++i)
			{
			const int ox=(rand()%(SPAWNER_SPREAD_DISTANCE*2+1))-SPAWNER_SPREAD_DISTANCE;
			const int oy=(rand()%(SPAWNER_SPREAD_DISTANCE*2+1))-SPAWNER_SPREAD_DISTANCE;

			const VectorI newFPosition=fposition+VectorI(ox, oy);

			if(!canSpreadToField(level, newFPosition))
				continue;

			if(!level->buildTurret(newFPosition, TurretType::ENEMY_SPAWNER))
				{
				LOG_WARNING("Nie udalo sie zasiedlic pola %d,%d", newFPosition.x, newFPosition.y);
				break;
				}

			LOG_INFO("Zasiedlono pole %d,%d", newFPosition.x, newFPosition.y);

			good=true;
			break;
			}

		if(!good)
			{
			LOG_WARNING("Nie udalo sie losowo znalezc dobrego pola do zasiedlenia, grrr");

			for(int oy=-SPAWNER_SPREAD_DISTANCE; oy<=SPAWNER_SPREAD_DISTANCE; ++oy)
				{
				for(int ox=-SPAWNER_SPREAD_DISTANCE; ox<=SPAWNER_SPREAD_DISTANCE; ++ox)
					{
					const VectorI newFPosition=fposition+VectorI(ox, oy);

					if(!canSpreadToField(level, newFPosition))
						continue;

					if(!level->buildTurret(newFPosition, TurretType::ENEMY_SPAWNER))
						{
						LOG_WARNING("Nie udalo sie zasiedlic pola %d,%d", newFPosition.x, newFPosition.y);
						break;
						}

					LOG_INFO("Zasiedlono pole %d,%d (nie losowo)", newFPosition.x, newFPosition.y);

					good=true;
					break;
					}

				if(good)
					break;
				}

			if(!good)
				{
				LOG_WARNING("Nie znaleziono pola zdatnego do zasiedlenia");
				return initStateOvercharge();
				}

			return;
			}

		cooldown=SPAWNER_COOLDOWN_SPREAD_POST;
		state=STATE_SPREADING_POST;
		}
	else
		{
		if(cooldown>0.0f)
			return;

		return initStateNormal();
		}
	}

void TSpawner::updateStatePanic(float dt)
	{
	if(cooldown>0.0f)
		return;

	if(!isRuleEnabled(RULE_ENEMY_SPAWN_UNITS))
		{
		cooldown=1.0f;
		}
	else if(waveUnit>=(waveCurDef->uinfantry+waveCurDef->uarmored)*4)
		{
		if(!isFieldExtraSafe(level, fposition))
			{
			hp=0.0f;
			return;
			}

		hp=0.75f;

		return initStateNormal();
		}
	else
		{
		const int ALL_UNITS_IN_WAVE=waveCurDef->uinfantry+waveCurDef->uarmored;

		cooldown=waveCurDef->cooldownUnit*0.25;

		if((waveUnit%ALL_UNITS_IN_WAVE)<waveCurDef->uarmored)
			level->spawnUnit(UnitType::ENEMY_ARMORED, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp*2.0f, level->getFieldDiagonalSize()*0.75f);
		else
			level->spawnUnit(UnitType::ENEMY_INFANTRY, fposition, Engine::Math::VectorI(0, 0), waveCurDef->hp*2.0f, level->getFieldDiagonalSize()*(0.75f + wave/20*0.2f));

		++waveUnit;
		}
	}

void TSpawner::updateStateOvercharge(float dt)
	{
	using namespace Engine::Math;

	//LOG_DEBUG("[wave %d][cd %.2f/%.2f]", wave, cooldown, waveCurDef->cooldownWave);

	if(spreadCooldown>0.0f)
		spreadCooldown-=dt;

	if(cooldown>0.0f)
		return;

	if(!isFieldSafe(level, fposition) || hp<0.5f)
		{
		return initStatePanic();
		}

	if(!isRuleEnabled(RULE_ENEMY_SPAWN_UNITS))
		{
		cooldown=1.0f;
		}
	else
		{
		if(wave>10)
			{
			hp-=0.4f;
			return initStateNormal();
			}

		cooldown=std::max(0.75f - 0.5f*wave, 0.5f);

		level->spawnUnit(UnitType::ENEMY_ARMORED, fposition, Engine::Math::VectorI(0, 0), 20.0f, level->getFieldDiagonalSize()*0.4f);

		++waveUnit;

		if(waveUnit>=10+wave*2)
			{
			cooldown*=4.0f;
			++wave;
			}
		}
	}


bool TSpawner::canSpread()
	{
	using namespace Engine::Math;

	for(int oy=-SPAWNER_SPREAD_DISTANCE; oy<=SPAWNER_SPREAD_DISTANCE; ++oy)
		{
		for(int ox=-SPAWNER_SPREAD_DISTANCE; ox<=SPAWNER_SPREAD_DISTANCE; ++ox)
			{
			if(std::abs(oy)==SPAWNER_SPREAD_DISTANCE && std::abs(ox)==SPAWNER_SPREAD_DISTANCE)
				continue;

			if(canSpreadToField(level, fposition+VectorI(ox, oy)))
				return true;
			}
		}

	return false;
	}

bool TSpawner::canSpreadToField(const Level* level, const Engine::Math::VectorI& fposition)
	{
	using namespace Engine::Math;

	const Level::Field* field=level->getField(fposition);

	if(!field)
		return false;
	if(field->turret)// && field->turret->getType()!=TurretType::PLAYER_CARROT_FIELD)
		return false;

	if(!isFieldExtraSafe(level, fposition))
		return false;

	return true;
	}

int TSpawner::getNearbySpawnerCount(const Level* level, const Engine::Math::VectorI& fposition)
	{
	using namespace Engine::Math;

	int ret=0;

	for(int oy=-1; oy<=1; ++oy)
		{
		for(int ox=-1; ox<=1; ++ox)
			{
			if(oy==0 && ox==0)
				continue;

			const Level::Field* field=level->getField(fposition);

			if(!field)
				return false;
			if(field->turret && field->turret->getType()==TurretType::ENEMY_SPAWNER)
				++ret;
			}
		}

	return ret;
	}
