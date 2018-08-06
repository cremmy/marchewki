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

	Render::getInstance().draw(cam.getBillboard(position), sprite);
	}

