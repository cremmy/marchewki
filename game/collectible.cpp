/*
 * collectible.cpp
 *
 *  Created on: 7 sie 2018
 *      Author: crm
 */

#include "collectible.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

using namespace Game;

bool Collectible::init(const Engine::Math::Vector& position, float value)
	{
	if(!(sprite=Engine::Graphics::SpritePtr("sprite/collectible.xml")))
		{
		LOG_ERROR("Nie udalo sie wczytac sprite");
		return false;
		}

	this->position=position;
	this->value=value;

	return true;
	}

void Collectible::update(float dt)
	{
	sprite.update(dt);

	if(collected)
		{
		position.z+=128.0f*dt;

		if(position.z>256.0f)
			{
			alive=false;
			}
		}
	}

void Collectible::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	Render::getInstance().draw(cam.getBillboard(position), sprite);
	}

