/*
 * particleemitter.h
 *
 *  Created on: 26 sie 2018
 *      Author: crm
 */

#pragma once

#include <vector>

#include "../engine/math/math.h"
#include "../engine/graphics/spriteptr.h"

namespace Game
	{
	enum class ParticleEmitterType
		{
		EXPLOSION,
		LINEAR_UP,
		};

	class ParticleEmitter
		{
		class Particle
			{
			protected:
				float ttl;
				float maxTTL;

				Engine::Math::Vector position;
				Engine::Math::Vector speed;

			public:
				Particle(float ttl, const Engine::Math::Vector& position, const Engine::Math::Vector& speed):
					ttl(ttl), maxTTL(ttl), position(position), speed(speed)
					{
					//
					}
				Particle(const Particle& p): ttl(p.ttl), maxTTL(p.maxTTL), position(p.position), speed(p.speed)
					{
					//
					}
				~Particle()
					{
					//
					}

				void update(float dt);

				bool isAlive() const {return ttl>0.0f;}
				Engine::Math::Vector getPosition() const {return position;}
				float getAlpha() const {return (ttl>maxTTL*0.1f)?1.0f:ttl/(maxTTL*0.1f);}
			};

			std::vector<Particle> particles;

			float ttl;
			float maxTTL;
			float accumulator;

			Engine::Graphics::SpritePtr sprite;
			Engine::Math::Vector position;
			ParticleEmitterType type;
			float rateINV;     // rate - cząstki/sekunda, rateINV -> odwrotność
			float radius;      // Odległość od środka emitera
			float minSpeed;    // Minimalna początkowa szybkość cząstek
			float maxSpeed;    // Maksymalna poczatkowa szybkość cząstek
			float particleTTL; // Czas życia cząstki

		public:
			ParticleEmitter():
				ttl(0.0f), maxTTL(0.0f), accumulator(0.0f),
				sprite(), position(), type(ParticleEmitterType::EXPLOSION), rateINV(1.0f), radius(0.0f), minSpeed(0.0f), maxSpeed(0.0f), particleTTL(0.0f)
				{
				//
				}
			ParticleEmitter(ParticleEmitterType type, const Engine::Math::Vector& position, float time, const Engine::Graphics::SpritePtr& sprite, float rate, float radius, float minSpeed, float maxSpeed, float particleTTL):
				ttl(0.0f), maxTTL(0.0f), accumulator(0.0f),
				sprite(), position(), type(ParticleEmitterType::EXPLOSION), rateINV(1.0f), radius(0.0f), minSpeed(0.0f), maxSpeed(0.0f), particleTTL(0.0f)
				{
				if(!init(type, position, time, sprite, rate, radius, minSpeed, maxSpeed, particleTTL))
					ttl=0.0f;
				}
			virtual ~ParticleEmitter()
				{
				//
				}

			bool init(ParticleEmitterType type, const Engine::Math::Vector& position, float time, const Engine::Graphics::SpritePtr& sprite, float rate, float radius, float minSpeed, float maxSpeed, float particleTTL);

			void update(float dt);
			void print(float tinterp);
			//void clear();

			bool isAlive() const {return ttl>0.0f || !particles.empty();}
		};

	} /* namespace Game */
