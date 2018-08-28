/*
 * particleemitter.cpp
 *
 *  Created on: 26 sie 2018
 *      Author: crm
 */

#include "particleemitter.h"

#include <cstdlib>

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

using namespace Game;

bool ParticleEmitter::init(ParticleEmitterType type, const Engine::Math::Vector& position, float time, const Engine::Graphics::SpritePtr& sprite, float rate, float radius, float minSpeed, float maxSpeed, float particleTTL)
	{
	if(!sprite)
		return false;
	if(rate<0.01f)
		{
		LOG_WARNING("Za niskie tempo generowania czastek: %f", rate);
		return false;
		}

	this->ttl=time;
	this->maxTTL=time;
	this->sprite=sprite;
	this->position=position;
	this->type=type;
	this->rateINV=1.0f/rate;
	this->radius=radius;
	this->minSpeed=minSpeed;
	this->maxSpeed=maxSpeed;
	this->particleTTL=particleTTL;

	// Przewidywana lizcba cząstek
	const int CREATED_PARTICLE_COUNT=ttl*rate;
	//LOG_DEBUG("Przewidywana liczba utworzonych czastek: %d", CREATED_PARTICLE_COUNT);
	particles.reserve(CREATED_PARTICLE_COUNT);

	return true;
	}


void ParticleEmitter::update(float dt)
	{
	using namespace Engine::Math;

	if(!isAlive())
		return;

	ttl-=dt;

	auto addParticle=[this](const Particle& nparticle)
		{
		bool found=false;
		for(auto& particle: particles)
			{
			if(particle.isAlive())
				continue;
			particle=nparticle;
			found=true;
			}
		if(!found)
			{
			particles.push_back(nparticle);
			}
		};

	// Update czastek
	for(auto& particle: particles)
		{
		if(!particle.isAlive())
			continue;

		particle.update(dt);
		}

	// Generowanie czastek
	if(ttl>0.0f)
		{
		accumulator+=dt;

		if(accumulator>50.0f*rateINV)
			{
			accumulator=50.0f*rateINV;
			}

		while(accumulator>rateINV)
			{
			accumulator-=rateINV;

			if(particles.size()>=particles.capacity())
				{
				accumulator=0.0f;
				break;
				}

			if(type==ParticleEmitterType::LINEAR_UP)
				{
				const float ANGLE=rand()/(RAND_MAX+1.0f)*2.0f*M_PI;
				const float DT_PART=rand()/(RAND_MAX+1.0f)*dt;
				const float SPEED=rand()/(RAND_MAX+1.0f)*(maxSpeed-minSpeed) + minSpeed;

				addParticle(
					Particle(
						particleTTL-DT_PART,
						position + Vector(cos(ANGLE), sin(ANGLE))*radius + Vector(0, 0, 1)*DT_PART,
						Vector(0, 0, 1)*SPEED
						));
				}
			else
				{
				const float ANGLE_HORZ=rand()/(RAND_MAX+1.0f)*2.0f*M_PI;
				const float ANGLE_VERT=rand()/(RAND_MAX+1.0f)*0.5f*M_PI*(0.1f+0.2f*ttl/maxTTL);
				const float DISTANCE=rand()/(RAND_MAX+1.0f)*radius*(0.5f+0.5f*ttl/maxTTL);
				const float DT_PART=rand()/(RAND_MAX+1.0f)*dt;
				const float SPEED=rand()/(RAND_MAX+1.0f)*(maxSpeed-minSpeed) + minSpeed;
				const Vector SPEED_DIR=Vector(cos(ANGLE_HORZ)*sin(ANGLE_VERT), sin(ANGLE_HORZ)*sin(ANGLE_VERT), cos(ANGLE_VERT)) * SPEED;

				addParticle(
					Particle(
						particleTTL-DT_PART,
						position + Vector(cos(ANGLE_HORZ), sin(ANGLE_HORZ))*DISTANCE + SPEED_DIR*DT_PART,
						SPEED_DIR
						));
				}
			}
		}

	// Kasowanie starych cząstek
	for(auto it=particles.begin(); it!=particles.end(); ++it)
		{
		if(it->isAlive())
			continue;

		particles.erase(it--);
		}
	}

void ParticleEmitter::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();
	const Orientation billboard=cam.getBillboard(Vector(0, 0));

	for(auto& particle: particles)
		{
		if(!particle.isAlive())
			continue;

		Render::getInstance().setColor(Vector(1, 1, 1, particle.getAlpha()));
		Render::getInstance().draw(billboard+particle.getPosition(), sprite);
		}

	Render::getInstance().setColor(Vector(1, 1, 1, 1));
	}



void ParticleEmitter::Particle::update(float dt)
	{
	if(ttl<=0.0f)
		return;

	ttl-=dt;

	position+=speed*dt;
	speed.z-=128.0f*dt;

	if(position.z<=0.0f && speed.z<0.0f)
		{
		speed.z=-0.8f*speed.z;
		ttl*=0.75f;
		}
	}
