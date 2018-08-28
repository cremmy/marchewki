/*
 * pareaofeffect.cpp
 *
 *  Created on: 8 sie 2018
 *      Author: crm
 */

#include "pareaofeffect.h"

#include "../engine/debug/log.h"

#include "level.h"
#include "particleemitter.h"
#include "unit.h"

using namespace Game;

PAreaOfEffect::PAreaOfEffect(const Engine::Math::Vector& target, float damageRange, float damage, float maxSpeed):
	Projectile(DamageType::AOE, damage, maxSpeed), target(target), damageRange(damageRange), pathCur(0.0f), pathStep(0.0f), startPosition()
	{
	//
	}

PAreaOfEffect::~PAreaOfEffect()
	{
	//
	}


bool PAreaOfEffect::init()
	{
	sprite=Engine::Graphics::SpritePtr("sprite/projectile.xml");

	if(!sprite)
		{
		return false;
		}

	direction=Engine::Math::Vector(0, 0, 1);

	return true;
	}

void PAreaOfEffect::update(float dt)
	{
	using namespace Engine::Math;

	sprite.update(dt);

	auto hermite=[](float t, const Vector& p0, const Vector& m0, const Vector& p1, const Vector& m1)->Vector
		{
		const float T1=t;
		const float T2=t*t;
		const float T3=t*t*t;

		return (   2.0f*T3 - 3.0f*T2 + 1.0f    )*p0 +
		       (        T3 - 2.0f*T2 +       T1)*m0 +
			   ( - 2.0f*T3 + 3.0f*T2           )*p1 +
			   (        T3 -      T2           )*m1;
		};

	if(pathStep==0.0f)
		{
		/* Przelicz długość krzywej i ustal krok */
		startPosition=position;
		// targetPositin=target;
		// startTangent=Vector(0, 0, 1);
		// targetTangent=Vector(0, 0, 1);

		float pathLen=0.0f;

		Vector p0=startPosition;

		for(float t=0.0f; t<1.0f; t+=0.05f) // Dokładność całkowania
			{
			Vector p1=hermite(t, startPosition, Vector(0, 0, maxSpeed*4.0f), target, Vector(0, 0, -maxSpeed));
			pathLen+=VectorLength(p1-p0);
			p0=p1;
			}

		const float PATH_TIME=pathLen/maxSpeed;
		pathStep=1.0f/PATH_TIME;
		}

	const Vector newPosition=hermite(pathCur, startPosition, Vector(0, 0, maxSpeed*4.0f), target, Vector(0, 0, -maxSpeed));
	direction=VectorNormalize(newPosition-position);
	position=newPosition;
	pathCur+=dt*pathStep;

	if(pathCur>=1.0f)
		{
		std::vector<Unit*> units;

		if(level->findAllUnitsInRange(target, damageRange, units))
			{
			for(auto unit: units)
				{
				unit->damage(damageType, damage);
				}
			}

		//target->damage(damageType, damage);

		level->addEmitter(new ParticleEmitter(ParticleEmitterType::EXPLOSION, target, 0.25f, Engine::Graphics::SpritePtr("sprite/particle_red.xml"), 400, damageRange*0.5f, 32, 96, 0.5f));

		alive=false;
		return;
		}
	}
