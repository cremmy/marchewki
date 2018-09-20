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

#include "../gui_messages.h"

using namespace Game;
using namespace Game::State;

Menu::Menu(): wndMain(nullptr), btnMainNewGame(nullptr), btnMainTutorial(nullptr), btnMainOptions(nullptr), btnMainExit(nullptr),
	wndNewGame(nullptr), btnNewGameSandbox(nullptr), btnNewGameEasy(nullptr), btnNewGameMedium(nullptr), btnNewGameHard(nullptr), btnNewGameCustom(nullptr), btnNewGameBack(nullptr),
	wndCustom(nullptr), chkCustomRuleBuildCost(nullptr), chkCustomRuleEnemySpawn(nullptr), chkCustomRuleDrainHP(nullptr), chkCustomRuleDrainRes(nullptr), chkCustomRuleEnemySpread(nullptr),
	chkCustomRulePreferCarrots(nullptr), chkCustomRuleAvoidTurrets(nullptr), chkCustomRulePlayerStomp(nullptr), btnCustomStart(nullptr), btnCustomBack(nullptr),
	wndOptions(nullptr), chkOptionsSounds(nullptr), chkOptionsMusic(nullptr), btnOptionsBack(nullptr),
	currentWindow(nullptr), receiver(0)
	{
	using namespace Engine::Render;

	const int W=Render::getInstance().getWindowWidth();
	const int H=Render::getInstance().getWindowHeight();

	const int BTN_W=256;
	const int BTN_H=32;

	wndMain=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnMainNewGame=new UI::Button(BTN_W, BTN_H, MENU_BTN_NEW_GAME, &receiver, IFACE_GOTO_NEW_GAME);
	btnMainTutorial=new UI::Button(BTN_W, BTN_H, MENU_BTN_TUTORIAL, &receiver, IFACE_START_TUTORIAL);
	btnMainOptions=new UI::Button(BTN_W, BTN_H, MENU_BTN_OPTIONS, &receiver, IFACE_GOTO_OPTIONS);
	btnMainExit=new UI::Button(BTN_W, BTN_H, MENU_BTN_EXIT, &receiver, IFACE_EXIT);
	wndMain->addChild(btnMainNewGame, {(W-BTN_W)/2, BTN_H*2}, true);
	wndMain->addChild(btnMainTutorial, {(W-BTN_W)/2, BTN_H*3}, true);
	wndMain->addChild(btnMainOptions, {(W-BTN_W)/2, BTN_H*4}, true);
	wndMain->addChild(btnMainExit, {(W-BTN_W)/2, BTN_H*5}, true);

	wndNewGame=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnNewGameSandbox;
	btnNewGameEasy;
	btnNewGameMedium;
	btnNewGameHard;
	btnNewGameCustom;
	btnNewGameBack;

	wndCustom=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	chkCustomRuleBuildCost;
	chkCustomRuleEnemySpawn;
	chkCustomRuleDrainHP;
	chkCustomRuleDrainRes;
	chkCustomRuleEnemySpread;
	chkCustomRulePreferCarrots;
	chkCustomRuleAvoidTurrets;
	chkCustomRulePlayerStomp;
	btnCustomStart;
	btnCustomBack;

	wndOptions=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	chkOptionsSounds;
	chkOptionsMusic;
	btnOptionsBack;

	currentWindow=wndMain;
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

	int mx;
	int my;
	SDL_GetMouseState(&mx, &my);

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				application->stop();
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==1)
			{
			// Czy kliknięto w któryś przycisk?
			if(currentWindow->click({e.data.mouse.x, e.data.mouse.y}))
				{
				// TODO Sygnał dźwiękowy kliknięcia w przycisk?
				}
			}
		}

	currentWindow->hover({mx, my});
	currentWindow->update(dt);

	if(receiver!=0)
		{
		if(currentWindow==wndMain)
			{
			if(receiver&IFACE_GOTO_NEW_GAME)
				{
				//
				}
			else if(receiver&IFACE_START_TUTORIAL)
				{
				//
				}
			else if(receiver&IFACE_GOTO_OPTIONS)
				{
				//
				}
			else if(receiver&IFACE_EXIT)
				{
				application->stop();
				}
			}
		else if(currentWindow==wndNewGame)
			{
			//
			}
		else if(currentWindow==wndCustom)
			{
			//
			}
		else if(currentWindow==wndOptions)
			{
			//
			}

		receiver=0;
		}

	return false;
	}

void Menu::print(float tinterp)
	{
	using namespace Engine::Render;

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	currentWindow->print(tinterp);
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

