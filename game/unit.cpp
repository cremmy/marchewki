/*
 * unit.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "unit.h"

#include "../engine/render/render.h"

using namespace Game;

void Unit::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	/*const Vector directionRelative=VectorNormalize(cam.getView()*(target-position));
	const float angle=(directionRelative.x>0.0f)?(360.0f-acos(directionRelative.y)*180.0f/M_PI) : (acos(directionRelative.y)*180.0f/M_PI);

	sprite.setDirection(Engine::Graphics::Sprite::DirectionName::UP+angle);*/
	if(((target-position)|cam.getRight())<0.0f)
		sprite.setDirection(Engine::Graphics::Sprite::DirectionName::LEFT);
	else
		sprite.setDirection(Engine::Graphics::Sprite::DirectionName::RIGHT);

	Render::getInstance().draw(cam.getBillboard(position), sprite);
	}

