/*
 * checkbutton.cpp
 *
 *  Created on: 20 wrz 2018
 *      Author: crm
 */

#include "checkbutton.h"

#include "../../engine/debug/log.h"
#include "../../engine/render/render.h"

using namespace Game::UI;

void CheckButton::update(float dt)
	{
	//
	}

void CheckButton::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	if(!enabled)
		{
		Render::getInstance().setColor(Vector(0.5f, 0.5f, 0.5f, 1.0f));
		}
	else if(checked)
		{
		Render::getInstance().setColor(Vector(0.5f, 1.0f, 0.5f, 1.0f));
		}
	else if(wasHovered)
		{
		Render::getInstance().setColor(Vector(0.95f, 1.0f, 0.95f, 1.0f));
		}

	if(sprite)
		Render::getInstance().draw(Orientation::GUI+Vector(position.x, position.y), sprite);
	if(!text.isEmpty())
		text.print(Orientation::GUI+Vector(position.x, position.y));
	Render::getInstance().setColor(Vector(1, 1, 1, 1));

	wasHovered=false;
	}


bool CheckButton::click(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;
	if(!enabled)
		return true; // Kliknięty poprawnie, więc zwracam true

	checked=!checked;

	return true;
	}

bool CheckButton::hover(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;
	if(!enabled)
		return true;

	wasHovered=true;

	return true;
	}

