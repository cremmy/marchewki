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
#include "../../engine/sound/soundplayer.h"

#include "../state/help.h"
#include "../gui_messages.h"
#include "../rules.h"

using namespace Game;
using namespace Game::State;

MenuIngame::MenuIngame(): wndMain(nullptr), btnMainResume(nullptr), btnMainHelp(nullptr), btnMainRules(nullptr), btnMainExitMenu(nullptr), btnMainExitOS(nullptr),
	wndRules(nullptr), chkRulesBuildCost(nullptr), chkRulesEnemySpawn(nullptr), chkRulesDrainHP(nullptr), chkRulesDrainRes(nullptr),
	chkRulesEnemySpread(nullptr), chkRulesPreferCarrots(nullptr), chkRulesAvoidTurrets(nullptr), chkRulesPlayerStomp(nullptr),
	btnCustomResume(nullptr), btnCustomBack(nullptr), currentWindow(nullptr), receiver(0)
	{
	using namespace Engine::Render;

	const int W=Render::getInstance().getWindowWidth();
	const int H=Render::getInstance().getWindowHeight();

	const int BTN_W=640;
	const int BTN_H=32;

	wndMain=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnMainResume=new UI::Button(BTN_W, BTN_H, MENU_BTN_RESUME_GAME, &receiver, IFACE_RESUME_GAME);
	btnMainHelp=new UI::Button(BTN_W, BTN_H, MENU_BTN_HELP, &receiver, IFACE_GOTO_HELP);
	btnMainRules=new UI::Button(BTN_W, BTN_H, MENU_BTN_RULES, &receiver, IFACE_GOTO_RULES);
	btnMainExitMenu=new UI::Button(BTN_W, BTN_H, MENU_BTN_RETURN_TO_MENU, &receiver, IFACE_GOTO_MENU);
	btnMainExitOS=new UI::Button(BTN_W, BTN_H, MENU_BTN_RETURN_TO_OS, &receiver, IFACE_GOTO_OS);
	wndMain->addChild(btnMainResume,   {(W-BTN_W)/2, BTN_H*2}, true);
	wndMain->addChild(btnMainHelp,     {(W-BTN_W)/2, BTN_H*3}, true);
	wndMain->addChild(btnMainRules,    {(W-BTN_W)/2, BTN_H*4}, true);
	wndMain->addChild(btnMainExitMenu, {(W-BTN_W)/2, BTN_H*6}, true);
	wndMain->addChild(btnMainExitOS,   {(W-BTN_W)/2, BTN_H*7}, true);

	wndRules=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	chkRulesBuildCost=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_BUILD_COST, isRuleEnabled(RULE_BUILDING_COST));
	chkRulesEnemySpawn=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_ENEMY_SPAWN, isRuleEnabled(RULE_ENEMY_SPAWN_UNITS));
	chkRulesDrainHP=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_DRAIN_HP, isRuleEnabled(RULE_DRAIN_HP));
	chkRulesDrainRes=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_DRAIN_RESOURCES, isRuleEnabled(RULE_DRAIN_RESOURCES));
	chkRulesEnemySpread=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_ENEMY_SPREAD, isRuleEnabled(RULE_ENEMY_BUILD_TURRETS));
	chkRulesPreferCarrots=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_PREFER_CARROTS, isRuleEnabled(RULE_ENEMY_PREFER_CARROTS));
	chkRulesAvoidTurrets=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_AVOID_TURRETS, isRuleEnabled(RULE_ENEMY_AVOID_TURRETS));
	chkRulesPlayerStomp=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_PLAYER_STOMP, isRuleEnabled(RULE_PLAYER_STOMPS_CARROTS));
	btnCustomResume=new UI::Button(BTN_W, BTN_H, MENU_BTN_RESUME_GAME, &receiver, IFACE_RESUME_GAME);
	btnCustomBack=new UI::Button(BTN_W, BTN_H, MENU_BTN_BACK, &receiver, IFACE_BACK);
	wndRules->addChild(chkRulesBuildCost,     {(W-BTN_W)/2, BTN_H* 2}, true);
	wndRules->addChild(chkRulesEnemySpawn,    {(W-BTN_W)/2, BTN_H* 3}, true);
	wndRules->addChild(chkRulesDrainHP,       {(W-BTN_W)/2, BTN_H* 4}, true);
	wndRules->addChild(chkRulesDrainRes,      {(W-BTN_W)/2, BTN_H* 5}, true);
	wndRules->addChild(chkRulesEnemySpread,   {(W-BTN_W)/2, BTN_H* 6}, true);
	wndRules->addChild(chkRulesPreferCarrots, {(W-BTN_W)/2, BTN_H* 7}, true);
	wndRules->addChild(chkRulesAvoidTurrets,  {(W-BTN_W)/2, BTN_H* 8}, true);
	wndRules->addChild(chkRulesPlayerStomp,   {(W-BTN_W)/2, BTN_H* 9}, true);
	wndRules->addChild(btnCustomResume,       {(W-BTN_W)/2, BTN_H*11}, true);
	wndRules->addChild(btnCustomBack,         {(W-BTN_W)/2, BTN_H*13}, true);

	currentWindow=wndMain;
	}

MenuIngame::~MenuIngame()
	{
	delete wndMain;
	delete wndRules;
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
			if(currentWindow->click({e.data.mouse.x, e.data.mouse.y}))
				{
				Engine::Sound::getInstance().play("sounds/gui_click.ogg");
				}
			}
		}

	currentWindow->hover({mx, my});
	currentWindow->update(dt);

	if(receiver!=0)
		{
		if(currentWindow==wndMain)
			{
			if((receiver&IFACE_RESUME_GAME) || (receiver&IFACE_BACK))
				{
				application->popStateSafe();
				return false;
				}
			else if(receiver&IFACE_GOTO_HELP)
				{
				application->pushState(new State::Help());
				}
			else if(receiver&IFACE_GOTO_RULES)
				{
				currentWindow=wndRules;

				chkRulesBuildCost->setChecked(isRuleEnabled(RULE_BUILDING_COST));
				chkRulesEnemySpawn->setChecked(isRuleEnabled(RULE_ENEMY_SPAWN_UNITS));
				chkRulesDrainHP->setChecked(isRuleEnabled(RULE_DRAIN_HP));
				chkRulesDrainRes->setChecked(isRuleEnabled(RULE_DRAIN_RESOURCES));
				chkRulesEnemySpread->setChecked(isRuleEnabled(RULE_ENEMY_BUILD_TURRETS));
				chkRulesPreferCarrots->setChecked(isRuleEnabled(RULE_ENEMY_PREFER_CARROTS));
				chkRulesAvoidTurrets->setChecked(isRuleEnabled(RULE_ENEMY_AVOID_TURRETS));
				chkRulesPlayerStomp->setChecked(isRuleEnabled(RULE_PLAYER_STOMPS_CARROTS));
				}
			else if(receiver&IFACE_GOTO_MENU)
				{
				application->popStateSafe();
				application->popStateSafe();
				return false;
				}
			else if(receiver&IFACE_GOTO_OS)
				{
				application->stop();
				return false;
				}
			}
		else if(currentWindow==wndRules)
			{
			if(receiver&(IFACE_BACK|IFACE_RESUME_GAME))
				{
				setRuleset(RULE_NO_RULES);
				if(chkRulesBuildCost->isChecked()) enableRule(RULE_BUILDING_COST);
				if(chkRulesEnemySpawn->isChecked()) enableRule(RULE_ENEMY_SPAWN_UNITS);
				if(chkRulesDrainHP->isChecked()) enableRule(RULE_DRAIN_HP);
				if(chkRulesDrainRes->isChecked()) enableRule(RULE_DRAIN_RESOURCES);
				if(chkRulesEnemySpread->isChecked()) enableRule(RULE_ENEMY_BUILD_TURRETS);
				if(chkRulesPreferCarrots->isChecked()) enableRule(RULE_ENEMY_PREFER_CARROTS);
				if(chkRulesAvoidTurrets->isChecked()) enableRule(RULE_ENEMY_AVOID_TURRETS);
				if(chkRulesPlayerStomp->isChecked()) enableRule(RULE_PLAYER_STOMPS_CARROTS);

				if(receiver&IFACE_BACK)
					{
					currentWindow=wndMain;
					}
				else
					{
					application->popStateSafe();
					return false;
					}
				}
			}

		receiver=0;
		}

	return false;
	}

void MenuIngame::print(float tinterp)
	{
	using namespace Engine::Render;

	const float W=Render::getInstance().getWindowWidth();
	const float H=Render::getInstance().getWindowHeight();

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	Render::getInstance().drawPolygon(
		{
			{0.0f, 0.0f, 0.0f},
			{   W, 0.0f, 0.0f},
			{   W,    H, 0.0f},
			{0.0f,    H, 0.0f}
		},
		{0.0f, 0.0f, 0.0f, 0.5f},
		{0.0f, 0.0f, 0.0f, 0.5f});

	currentWindow->print(tinterp);
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
