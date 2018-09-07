/*
 * window.cpp
 *
 *  Created on: 30 lip 2018
 *      Author: crm
 */

#include "window.h"

#include "../../engine/render/render.h"

using namespace Game::UI;

void Window::update(float dt)
	{
	background.update(dt);

	Panel::update(dt);
	}

void Window::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	// TODO sprawdzanie czy resizeBackground jest prawdą

	if(resizeBackground)
		{
		const Vector RESIZE_FACTOR(
			size.x/background.getCurrentFrame().getWidth(),
			size.y/background.getCurrentFrame().getHeight(),
			1,
			1);

		Render::getInstance().draw(
			Orientation(
				{(float)position.x, (float)position.y},
				Orientation::GUI,
				RESIZE_FACTOR),
			background);
		}
	else
		{
		for(int y=0; y<size.y; y+=background.getCurrentFrame().getHeight())
			{
			Render::getInstance().draw(Orientation::GUI+Vector(position.x, position.y+y), background);
			}
		}


	Panel::print(tinterp);
	}

