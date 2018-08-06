/*
 * projectile.cpp
 *
 *  Created on: 4 sie 2018
 *      Author: crm
 */

#include "projectile.h"

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

using namespace Game;

void Projectile::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	const Vector directionRelative=VectorNormalize(cam.getView()*direction);
	const float angle=(directionRelative.x>0.0f)?(360.0f-acos(directionRelative.y)*180.0f/M_PI) : (acos(directionRelative.y)*180.0f/M_PI);

	auto dname=Engine::Graphics::Sprite::DirectionName::UP+angle;

	LOG_DEBUG("[dir %+7.2f %+7.2f %+7.2f][drel %+7.2f %+7.2f][angle %8.2f][dname %s]",
		direction.x, direction.y, direction.z,
		directionRelative.x, directionRelative.y,
		angle, Engine::Graphics::Sprite::getDirectionName(dname).c_str());

	sprite.setDirection(dname);

	Render::getInstance().draw(cam.getBillboard(position), sprite);
	}
