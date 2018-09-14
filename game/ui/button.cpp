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

	if(!enabled)
		{
		Render::getInstance().setColor(Vector(0.5f, 0.5f, 0.5f, 1.0f));
		}
	else if(wasClicked)
		{
		Render::getInstance().setColor(Vector(0.5f, 1.0f, 0.5f, 1.0f));
		}
	else if(wasHovered)
		{
		Render::getInstance().setColor(Vector(0.95f, 1.0f, 0.95f, 1.0f));
		}

	Render::getInstance().draw(Orientation::GUI+Vector(position.x, position.y), sprite);
	Render::getInstance().setColor(Vector(1, 1, 1, 1));

	if(wasHovered && hoverBackground)
		{
		const int WINDOW_W=Render::getInstance().getWindowWidth();
		const int WINDOW_H=Render::getInstance().getWindowHeight();
		const int HOVER_W=hoverBackground.getCurrentFrame().getWidth();
		const int HOVER_H=hoverBackground.getCurrentFrame().getHeight();

		hoverPosition=VectorClamp(VectorI(128, 0), hoverPosition, VectorI(WINDOW_W-HOVER_W, WINDOW_H-HOVER_H));
		Render::getInstance().draw(Orientation::GUI+Vector(hoverPosition.x, hoverPosition.y), hoverBackground);
		hoverMessage.print(Orientation::GUI+Vector(hoverPosition.x+8, hoverPosition.y+8));
		}

	wasClicked=false;
	wasHovered=false;
	}


bool Button::click(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;
	if(!enabled)
		return true; // Kliknięty poprawnie, więc zwracam true

	wasClicked=true;

	if(receiver)
		{
		//LOG_DEBUG("[btn %p spr %s][receiver: %p][code %d]", this, sprite->getPath().c_str(), receiver, receiverCode);
		(*receiver)|=receiverCode;
		}

	return true;
	}

bool Button::hover(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;
	if(!enabled)
		return true;

	wasHovered=true;
	hoverPosition=position;

	return true;
	}

void Button::setHoverMessage(const std::string& text)
	{
	hoverBackground=Engine::Graphics::SpritePtr("sprite/gui_hover_bg.xml");
	hoverMessage.init("font/dejavu.xml", "", hoverBackground.getCurrentFrame().getWidth()-16, hoverBackground.getCurrentFrame().getHeight()-16);
	hoverMessage=text;
	hoverMessage.update();
	}
