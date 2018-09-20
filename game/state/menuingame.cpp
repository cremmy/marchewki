/*
 * menuingame.cpp
 *
 *  Created on: 20 wrz 2018
 *      Author: crm
 */

#include "menuingame.h"

#include <SDL2/SDL.h>

#include "../../engine/debug/log.h"
#include "../../engine/render/camera.h"
#include "../../engine/render/render.h"

using namespace Game;
using namespace Game::State;

MenuIngame::MenuIngame()
	{
	//
	}

MenuIngame::~MenuIngame()
	{
	//
	}


bool MenuIngame::init(Engine::Core::Application *application)
	{
	this->application=application;

	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);

	return true;
	}

bool MenuIngame::update(float dt)
	{
	using namespace Engine::Math;

	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				application->popStateSafe();
				}
			else if(e.data.keyboard.key==SDLK_q)
				{
				application->popStateSafe();
				application->popStateSafe();
				}
			}
		}

	return false;
	}

void MenuIngame::print(float tinterp)
	{
	//
	}

void MenuIngame::clear()
	{
	this->application->removeListener(*this);
	}

void MenuIngame::pause()
	{
	this->application->removeListener(*this);
	}

void MenuIngame::resume()
	{
	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	}
