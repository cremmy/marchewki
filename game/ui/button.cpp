/*
 * button.cpp
 *
 *  Created on: 31 lip 2018
 *      Author: crm
 */

#include "button.h"

#include "../../engine/debug/log.h"
#include "../../engine/render/render.h"

using namespace Game::UI;

void Button::update(float dt)
	{
	//
	}

void Button::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	if(wasClicked)
		{
		Render::getInstance().setColor(Vector(0.5f, 1.0f, 0.5f, 1.0f));
		}
	else if(wasHovered)
		{
		Render::getInstance().setColor(Vector(0.8f, 1.0f, 0.8f, 1.0f));
		}

	wasClicked=false;
	wasHovered=false;

	Render::getInstance().draw(Orientation::GUI+Vector(position.x, position.y), sprite);
	Render::getInstance().setColor(Vector(1, 1, 1, 1));
	}


bool Button::click(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;

	wasClicked=true;

	if(receiver)
		{
		LOG_DEBUG("[btn %p spr %s][receiver: %p][code %d]", this, sprite->getPath(), receiver, receiverCode);
		(*receiver)|=receiverCode;
		}

	return true;
	}

bool Button::hover(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;

	wasHovered=true;

	return true;
	}
