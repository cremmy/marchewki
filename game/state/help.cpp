/*
 * help.cpp
 *
 *  Created on: 25 wrz 2018
 *      Author: crm
 */

#include "help.h"

#include <SDL2/SDL.h>

#include "../../engine/debug/log.h"
#include "../../engine/render/render.h"
#include "../../engine/sound/soundplayer.h"

#include "../help_messages.h"

using namespace Game::State;


Help::Help(): currentMenu(Menu::MAIN), receiver(0), window(nullptr)
	{
	//
	}

Help::~Help()
	{
	delete window;
	}


bool Help::init(Engine::Core::Application *application)
	{
	this->application=application;

	application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);

	if(!text.init("font/dejavu.xml", "", Engine::Render::getInstance().getWindowWidth()-32, Engine::Render::getInstance().getWindowHeight()-32*3))
		{
		LOG_ERROR("Nie udalo sie zainicjowac tekstu");
		return false;
		}

	initMenu(Menu::MAIN);

	return true;
	}

bool Help::update(float dt)
	{
	using namespace Engine::Math;

	Engine::Core::AppEvent e;

	int mx;
	int my;
	SDL_GetMouseState(&mx, &my);

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				//application->popStateSafe();
				receiver|=IFACE_BACK;
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==1)
			{
			// Czy kliknięto w któryś przycisk?
			if(window->click({e.data.mouse.x, e.data.mouse.y}))
				{
				Engine::Sound::getInstance().play("sounds/gui_click.ogg");
				}
			}
		}

	window->hover({mx, my});
	window->update(dt);

	if(receiver!=0)
		{
		if(receiver&IFACE_BACK)
			{
			switch(currentMenu)
				{
				case Menu::BASICS:
				case Menu::TURRETS:
				case Menu::ENEMIES:
				case Menu::RULES:
					initMenu(Menu::MAIN);
				break;

				case Menu::TURRET_SINGLE:
				case Menu::TURRET_AOE:
				case Menu::TURRET_MINE:
				case Menu::TURRET_CARROTS:
				case Menu::TURRET_BASE:
				case Menu::TURRET_SPAWNER:
					initMenu(Menu::TURRETS);
				break;

				case Menu::MAIN:
				default:
					application->popStateSafe();
				break;
				}
			}
		else
			{
			if(receiver&IFACE_BASICS)
				{
				initMenu(Menu::BASICS);
				}
			else if(receiver&IFACE_TURRETS)
				{
				initMenu(Menu::TURRETS);
				}
			else if(receiver&IFACE_TURRET_SINGLE)
				{
				initMenu(Menu::TURRET_SINGLE);
				}
			else if(receiver&IFACE_TURRET_AOE)
				{
				initMenu(Menu::TURRET_AOE);
				}
			else if(receiver&IFACE_TURRET_MINE)
				{
				initMenu(Menu::TURRET_MINE);
				}
			else if(receiver&IFACE_TURRET_CARROTS)
				{
				initMenu(Menu::TURRET_CARROTS);
				}
			else if(receiver&IFACE_TURRET_BASE)
				{
				initMenu(Menu::TURRET_BASE);
				}
			else if(receiver&IFACE_TURRET_SPAWNER)
				{
				initMenu(Menu::TURRET_SPAWNER);
				}
			else if(receiver&IFACE_ENEMIES)
				{
				initMenu(Menu::ENEMIES);
				}
			else if(receiver&IFACE_RULES)
				{
				initMenu(Menu::RULES);
				}
			}

		receiver=0;
		}

	return false;
	}

void Help::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);

	window->print(tinterp);

	if(!text.isEmpty())
		{
		text.print(Orientation::GUI+Vector(16, 32*3));
		}
	}

void Help::clear()
	{
	this->application->removeListener(*this);
	}

void Help::pause()
	{
	//
	}

void Help::resume()
	{
	//
	}



bool Help::initMenu(Menu menu)
	{
	using namespace Engine::Render;

	LOG_DEBUG("Zmiana menu: %d -> %d", currentMenu, menu);
	currentMenu=menu;

	const int W=Render::getInstance().getWindowWidth();
	const int H=Render::getInstance().getWindowHeight();
	const int BTN_W=W;
	const int BTN_H=32;

	if(window)
		delete window;

	window=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	window->addChild(
		new UI::Button(
			BTN_W, BTN_H,
			MENU_BTN_BACK,
			&receiver, IFACE_BACK
			),
		{0, BTN_H*2},
		true);

	switch(menu)
		{
		case Menu::MAIN:
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_BASICS,
					&receiver, IFACE_BASICS
					),
				{0, BTN_H*4},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRETS,
					&receiver, IFACE_TURRETS
					),
				{0, BTN_H*5},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_ENEMIES,
					&receiver, IFACE_ENEMIES
					),
				{0, BTN_H*6},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_RULES,
					&receiver, IFACE_RULES
					),
				{0, BTN_H*7},
				true);

			text.setStr("");
			text.update();
		break;

		case Menu::BASICS:
			text.setStr(HELP_BASICS);
			text.update();
		break;

		case Menu::TURRETS:
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_SINGLE,
					&receiver, IFACE_TURRET_SINGLE
					),
				{0, BTN_H*4},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_AOE,
					&receiver, IFACE_TURRET_AOE
					),
				{0, BTN_H*5},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_MINE,
					&receiver, IFACE_TURRET_MINE
					),
				{0, BTN_H*6},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_CARROTS,
					&receiver, IFACE_TURRET_CARROTS
					),
				{0, BTN_H*7},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_BASE,
					&receiver, IFACE_TURRET_BASE
					),
				{0, BTN_H*8},
				true);
			window->addChild(
				new UI::Button(
					BTN_W, BTN_H,
					MENU_BTN_TURRET_SPAWNER,
					&receiver, IFACE_TURRET_SPAWNER
					),
				{0, BTN_H*9},
				true);

			text.setStr("");
			text.update();
		break;

		case Menu::ENEMIES:
			text.setStr(HELP_ENEMIES);
			text.update();
		break;

		case Menu::RULES:
			text.setStr(HELP_RULES);
			text.update();
		break;

		case Menu::TURRET_SINGLE:
			text.setStr(HELP_TURRET_SINGLE);
			text.update();
		break;

		case Menu::TURRET_AOE:
			text.setStr(HELP_TURRET_AOE);
			text.update();
		break;

		case Menu::TURRET_MINE:
			text.setStr(HELP_TURRET_MINE);
			text.update();
		break;

		case Menu::TURRET_CARROTS:
			text.setStr(HELP_TURRET_CARROTS);
			text.update();
		break;

		case Menu::TURRET_BASE:
			text.setStr(HELP_TURRET_BASE);
			text.update();
		break;

		case Menu::TURRET_SPAWNER:
			text.setStr(HELP_TURRET_SPAWNER);
			text.update();
		break;

		default:
			//
		break;
		}

	return true;
	}

