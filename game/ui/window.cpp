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

	// TODO sprawdzanie czy resizeBackground jest prawdą

	const Vector RESIZE_FACTOR(
		size.x/background.getCurrentFrame().getWidth(),
		size.y/background.getCurrentFrame().getHeight());

	Engine::Render::getInstance().draw(
		Orientation(
			{(float)position.x, (float)position.y},
			Orientation::GUI,
			RESIZE_FACTOR),
		background);


	Panel::print(tinterp);
	}

