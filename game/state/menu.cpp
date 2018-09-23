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
#include "../../engine/sound/soundplayer.h"

#include "../gui_messages.h"
#include "../rules.h"

#include "towerdefense.h"

using namespace Game;
using namespace Game::State;

Menu::Menu(): wndMain(nullptr), btnMainNewGame(nullptr), btnMainTutorial(nullptr), btnMainOptions(nullptr), btnMainHelp(nullptr), btnMainExit(nullptr),
	wndNewGame(nullptr), btnNewGameSandbox(nullptr), btnNewGameEasy(nullptr), btnNewGameMedium(nullptr), btnNewGameHard(nullptr), btnNewGameCustom(nullptr), btnNewGameBack(nullptr),
	wndCustom(nullptr), chkCustomRuleBuildCost(nullptr), chkCustomRuleEnemySpawn(nullptr), chkCustomRuleDrainHP(nullptr), chkCustomRuleDrainRes(nullptr), chkCustomRuleEnemySpread(nullptr),
	chkCustomRulePreferCarrots(nullptr), chkCustomRuleAvoidTurrets(nullptr), chkCustomRulePlayerStomp(nullptr), valCustomWidth(nullptr), valCustomHeight(nullptr), btnCustomStart(nullptr), btnCustomBack(nullptr),
	wndOptions(nullptr), chkOptionsSounds(nullptr), chkOptionsMusic(nullptr), btnOptionsBack(nullptr),
	wndHelp(nullptr), btnHelpBack(nullptr),
	currentWindow(nullptr), receiver(0)
	{
	using namespace Engine::Render;

	const int W=Render::getInstance().getWindowWidth();
	const int H=Render::getInstance().getWindowHeight();

	const int BTN_W=640;
	const int BTN_H=32;

	wndMain=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnMainNewGame=new UI::Button(BTN_W, BTN_H, MENU_BTN_NEW_GAME, &receiver, IFACE_GOTO_NEW_GAME);
	btnMainTutorial=new UI::Button(BTN_W, BTN_H, MENU_BTN_TUTORIAL, &receiver, IFACE_START_TUTORIAL);
	btnMainOptions=new UI::Button(BTN_W, BTN_H, MENU_BTN_OPTIONS, &receiver, IFACE_GOTO_OPTIONS);
	btnMainHelp=new UI::Button(BTN_W, BTN_H, MENU_BTN_HELP, &receiver, IFACE_GOTO_HELP);
	btnMainExit=new UI::Button(BTN_W, BTN_H, MENU_BTN_EXIT, &receiver, IFACE_EXIT);
	wndMain->addChild(btnMainNewGame,  {(W-BTN_W)/2, BTN_H*2}, true);
	wndMain->addChild(btnMainTutorial, {(W-BTN_W)/2, BTN_H*3}, true);
	//wndMain->addChild(btnMainOptions, {(W-BTN_W)/2, BTN_H*4}, true);
	wndMain->addChild(btnMainHelp,     {(W-BTN_W)/2, BTN_H*4}, true);
	wndMain->addChild(btnMainExit,     {(W-BTN_W)/2, BTN_H*6}, true);

	wndNewGame=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnNewGameSandbox=new UI::Button(BTN_W, BTN_H, MENU_BTN_GAME_SANDBOX, &receiver, IFACE_START_SANDBOX);
	btnNewGameEasy=new UI::Button(BTN_W, BTN_H, MENU_BTN_GAME_EASY, &receiver, IFACE_START_EASY);
	btnNewGameMedium=new UI::Button(BTN_W, BTN_H, MENU_BTN_GAME_MEDIUM, &receiver, IFACE_START_MEDIUM);
	btnNewGameHard=new UI::Button(BTN_W, BTN_H, MENU_BTN_GAME_HARD, &receiver, IFACE_START_HARD);
	btnNewGameCustom=new UI::Button(BTN_W, BTN_H, MENU_BTN_GAME_CUSTOM, &receiver, IFACE_GOTO_CUSTOM);
	btnNewGameBack=new UI::Button(BTN_W, BTN_H, MENU_BTN_BACK, &receiver, IFACE_BACK);
	wndNewGame->addChild(btnNewGameSandbox, {(W-BTN_W)/2, BTN_H*2}, true);
	wndNewGame->addChild(btnNewGameEasy,    {(W-BTN_W)/2, BTN_H*3}, true);
	wndNewGame->addChild(btnNewGameMedium,  {(W-BTN_W)/2, BTN_H*4}, true);
	wndNewGame->addChild(btnNewGameHard,    {(W-BTN_W)/2, BTN_H*5}, true);
	wndNewGame->addChild(btnNewGameCustom,  {(W-BTN_W)/2, BTN_H*6}, true);
	wndNewGame->addChild(btnNewGameBack,    {(W-BTN_W)/2, BTN_H*8}, true);

	wndCustom=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	chkCustomRuleBuildCost=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_BUILD_COST, isRuleEnabled(RULE_BUILDING_COST));
	chkCustomRuleEnemySpawn=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_ENEMY_SPAWN, isRuleEnabled(RULE_ENEMY_SPAWN_UNITS));
	chkCustomRuleDrainHP=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_DRAIN_HP, isRuleEnabled(RULE_DRAIN_HP));
	chkCustomRuleDrainRes=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_DRAIN_RESOURCES, isRuleEnabled(RULE_DRAIN_RESOURCES));
	chkCustomRuleEnemySpread=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_ENEMY_SPREAD, isRuleEnabled(RULE_ENEMY_BUILD_TURRETS));
	chkCustomRulePreferCarrots=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_PREFER_CARROTS, isRuleEnabled(RULE_ENEMY_PREFER_CARROTS));
	chkCustomRuleAvoidTurrets=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_AVOID_TURRETS, isRuleEnabled(RULE_ENEMY_AVOID_TURRETS));
	chkCustomRulePlayerStomp=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_RULE_PLAYER_STOMP, isRuleEnabled(RULE_PLAYER_STOMPS_CARROTS));
	valCustomWidth=new UI::ValueSelector(BTN_W, BTN_H, MENU_VAL_LEVEL_WIDTH, 4, 20, 7);
	valCustomHeight=new UI::ValueSelector(BTN_W, BTN_H, MENU_VAL_LEVEL_HEIGHT, 4, 20, 7);
	btnCustomStart=new UI::Button(BTN_W, BTN_H, MENU_BTN_START, &receiver, IFACE_START_CUSTOM);
	btnCustomBack=new UI::Button(BTN_W, BTN_H, MENU_BTN_BACK, &receiver, IFACE_BACK);
	wndCustom->addChild(chkCustomRuleBuildCost,     {(W-BTN_W)/2, BTN_H* 2}, true);
	wndCustom->addChild(chkCustomRuleEnemySpawn,    {(W-BTN_W)/2, BTN_H* 3}, true);
	wndCustom->addChild(chkCustomRuleDrainHP,       {(W-BTN_W)/2, BTN_H* 4}, true);
	wndCustom->addChild(chkCustomRuleDrainRes,      {(W-BTN_W)/2, BTN_H* 5}, true);
	wndCustom->addChild(chkCustomRuleEnemySpread,   {(W-BTN_W)/2, BTN_H* 6}, true);
	wndCustom->addChild(chkCustomRulePreferCarrots, {(W-BTN_W)/2, BTN_H* 7}, true);
	wndCustom->addChild(chkCustomRuleAvoidTurrets,  {(W-BTN_W)/2, BTN_H* 8}, true);
	wndCustom->addChild(chkCustomRulePlayerStomp,   {(W-BTN_W)/2, BTN_H* 9}, true);
	wndCustom->addChild(valCustomWidth,             {(W-BTN_W)/2, BTN_H*10}, true);
	wndCustom->addChild(valCustomHeight,            {(W-BTN_W)/2, BTN_H*11}, true);
	wndCustom->addChild(btnCustomStart,             {(W-BTN_W)/2, BTN_H*13}, true);
	wndCustom->addChild(btnCustomBack,              {(W-BTN_W)/2, BTN_H*15}, true);

	wndOptions=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	chkOptionsSounds=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_SOUNDS, Engine::Sound::getInstance().isSoundEnabled());
	chkOptionsMusic=new UI::CheckButton(BTN_W, BTN_H, MENU_CHK_MUSIC, Engine::Sound::getInstance().isMusicEnabled());
	btnOptionsBack=new UI::Button(BTN_W, BTN_H, MENU_BTN_BACK, &receiver, IFACE_BACK);
	wndOptions->addChild(chkOptionsSounds, {(W-BTN_W)/2, BTN_H*2}, true);
	wndOptions->addChild(chkOptionsMusic,  {(W-BTN_W)/2, BTN_H*3}, true);
	wndOptions->addChild(btnOptionsBack,   {(W-BTN_W)/2, BTN_H*5}, true);

	wndHelp=new UI::Window({W, H}, Engine::Graphics::SpritePtr());
	btnHelpBack=new UI::Button(BTN_W, BTN_H, MENU_BTN_BACK, &receiver, IFACE_BACK);
	wndHelp->addChild(btnHelpBack, {(W-BTN_W)/2, BTN_H*2}, true);

	currentWindow=wndMain;
	}

Menu::~Menu()
	{
	delete wndMain;
	delete wndNewGame;
	delete wndCustom;
	delete wndOptions;
	}


bool Menu::init(Engine::Core::Application *application)
	{
	this->application=application;

	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);

	background=Engine::Graphics::ImagePtr("image/menu_bg.png");
	if(!background)
		return false;

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
				currentWindow=wndNewGame;
				}
			else if(receiver&IFACE_START_TUTORIAL)
				{
				//
				}
			else if(receiver&IFACE_GOTO_OPTIONS)
				{
				currentWindow=wndOptions;
				}
			else if(receiver&IFACE_GOTO_HELP)
				{
				currentWindow=wndHelp;
				}
			else if(receiver&IFACE_EXIT)
				{
				application->stop();
				}
			}
		else if(currentWindow==wndNewGame)
			{
			if(receiver&IFACE_START_SANDBOX)
				{
				setRuleset(RULESET_SANDBOX);
				application->pushState(new TowerDefense(9001, 1, 20, 20));
				}
			else if(receiver&IFACE_START_EASY)
				{
				setRuleset(RULESET_EASY);
				application->pushState(new TowerDefense(6.66, 7, 7, 7));
				}
			else if(receiver&IFACE_START_MEDIUM)
				{
				setRuleset(RULESET_EASY);
				application->pushState(new TowerDefense(6.66, 6, 14, 14));
				}
			else if(receiver&IFACE_START_HARD)
				{
				setRuleset(RULESET_HARD);
				application->pushState(new TowerDefense(6.66, 5, 20, 20));
				}
			else if(receiver&IFACE_GOTO_CUSTOM)
				{
				currentWindow=wndCustom;

				chkCustomRuleBuildCost->setChecked(isRuleEnabled(RULE_BUILDING_COST));
				chkCustomRuleEnemySpawn->setChecked(isRuleEnabled(RULE_ENEMY_SPAWN_UNITS));
				chkCustomRuleDrainHP->setChecked(isRuleEnabled(RULE_DRAIN_HP));
				chkCustomRuleDrainRes->setChecked(isRuleEnabled(RULE_DRAIN_RESOURCES));
				chkCustomRuleEnemySpread->setChecked(isRuleEnabled(RULE_ENEMY_BUILD_TURRETS));
				chkCustomRulePreferCarrots->setChecked(isRuleEnabled(RULE_ENEMY_PREFER_CARROTS));
				chkCustomRuleAvoidTurrets->setChecked(isRuleEnabled(RULE_ENEMY_AVOID_TURRETS));
				chkCustomRulePlayerStomp->setChecked(isRuleEnabled(RULE_PLAYER_STOMPS_CARROTS));
				}
			else if(receiver&IFACE_BACK)
				{
				currentWindow=wndMain;
				}
			}
		else if(currentWindow==wndCustom)
			{
			if(receiver&IFACE_START_CUSTOM)
				{
				setRuleset(RULE_NO_RULES);
				if(chkCustomRuleBuildCost->isChecked()) enableRule(RULE_BUILDING_COST);
				if(chkCustomRuleEnemySpawn->isChecked()) enableRule(RULE_ENEMY_SPAWN_UNITS);
				if(chkCustomRuleDrainHP->isChecked()) enableRule(RULE_DRAIN_HP);
				if(chkCustomRuleDrainRes->isChecked()) enableRule(RULE_DRAIN_RESOURCES);
				if(chkCustomRuleEnemySpread->isChecked()) enableRule(RULE_ENEMY_BUILD_TURRETS);
				if(chkCustomRulePreferCarrots->isChecked()) enableRule(RULE_ENEMY_PREFER_CARROTS);
				if(chkCustomRuleAvoidTurrets->isChecked()) enableRule(RULE_ENEMY_AVOID_TURRETS);
				if(chkCustomRulePlayerStomp->isChecked()) enableRule(RULE_PLAYER_STOMPS_CARROTS);

				application->pushState(new TowerDefense(6.66, 5, valCustomWidth->getValue(), valCustomHeight->getValue()));
				}
			else if(receiver&IFACE_BACK)
				{
				currentWindow=wndNewGame;
				}
			}
		else if(currentWindow==wndOptions)
			{
			if(receiver&IFACE_BACK)
				{
				currentWindow=wndMain;
				}
			}
		else if(currentWindow==wndHelp)
			{
			if(receiver&IFACE_BACK)
				{
				currentWindow=wndMain;
				}
			}

		receiver=0;
		}

	return false;
	}

void Menu::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	const int W=Render::getInstance().getWindowWidth();
	//const int H=Render::getInstance().getWindowHeight();
	const int BG_W=background->getW();
	//const int BG_H=background->getH();

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	Render::getInstance().draw(Orientation::GUI+Vector((W-BG_W)/2, 0), background);

	currentWindow->print(tinterp);
	}

void Menu::clear()
	{
	this->application->removeListener(*this);
	}

void Menu::pause()
	{
	this->application->removeListener(*this);

	background=nullptr;
	}

void Menu::resume()
	{
	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);

	background=Engine::Graphics::ImagePtr("image/menu_bg.png");
	}

