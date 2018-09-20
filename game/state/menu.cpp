/*
 * menu.cpp
 *
 *  Created on: 20 wrz 2018
 *      Author: crm
 */

#include "menu.h"

#include <SDL2/SDL.h>

#include "../../engine/debug/log.h"
#include "../../engine/render/camera.h"
#include "../../engine/render/render.h"

using namespace Game;
using namespace Game::State;

Menu::Menu()
	{
	//
	}

Menu::~Menu()
	{
	//
	}


bool Menu::init(Engine::Core::Application *application)
	{
	this->application=application;

	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);

	return true;
	}

bool Menu::update(float dt)
	{
	using namespace Engine::Math;

	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				application->stop();
				}
			}
		}

	return false;
	}

void Menu::print(float tinterp)
	{
	//
	}

void Menu::clear()
	{
	this->application->removeListener(*this);
	}

void Menu::pause()
	{
	this->application->removeListener(*this);
	}

void Menu::resume()
	{
	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	}

