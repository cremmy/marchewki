/*
 * window.cpp
 *
 *  Created on: 30 lip 2018
 *      Author: crm
 */

#include "window.h"

using namespace Game::UI;

void Window::update(float dt)
	{
	background.update(dt);

	Panel::update(dt);
	}

void Window::print(float tinterp)
	{
	using namespace Engine::Math;

	const Vector RESIZE_FACTOR(
		size.x/background.getCurrentFrame().getWidth(),
		size.y/background.getCurrentFrame().getHeight());

	Panel::print(tinterp);
	}

