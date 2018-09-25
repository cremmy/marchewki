/*
 * tutorial.cpp
 *
 *  Created on: 25 wrz 2018
 *      Author: crm
 */

#include "tutorial.h"

#include "../../engine/debug/log.h"
#include "../../engine/render/camera.h"
#include "../../engine/render/render.h"

#include "../rules.h"
#include "../tutorial_messages.h"
#include "../ui/button.h"
#include "../ui/window.h"

using namespace Game::State;

Tutorial::Tutorial(): TowerDefense(666.0f, 100.0f, 7, 7),
	tutorialStep(0), ifaceReceiverTutorial(0), ifaceBtnTutorialNext(nullptr)
	{
	//
	}

Tutorial::~Tutorial()
	{
	//
	}


bool Tutorial::init(Engine::Core::Application *application)
	{
	using namespace Engine::Math;

	if(!TowerDefense::init(application))
		{
		return false;
		}

	setRuleset(Rules::RULE_NO_RULES);

	ifaceBtnTutorialNext=new UI::Button(Engine::Graphics::SpritePtr("sprite/gui_btn_next.xml"), &ifaceReceiverTutorial, 1);
	interface->addChild(ifaceBtnTutorialNext, {8, 96+96*6+32}, true);

	if(!ifaceTutorialText.init("font/dejavu.xml", "", Engine::Render::getInstance().getWindowWidth()-128, Engine::Render::getInstance().getWindowHeight()))
		{
		LOG_ERROR("Nie udalo sie zainicjowac tekstu");
		return false;
		}

	return true;
	}

bool Tutorial::update(float dt)
	{
	TowerDefense::update(dt);

	if(ifaceReceiverTutorial || tutorialStep==0)
		{
		++tutorialStep;

		switch(tutorialStep)
			{
			case 1:
				ifaceTutorialText.setStr(TUTORIAL_01);
			break;

			case 2:
				enableRule(RULE_BUILDING_COST);
				ifaceTutorialText.setStr(TUTORIAL_02);
			break;

			case 3:
				enableRule(RULE_ENEMY_SPAWN_UNITS);
				ifaceTutorialText.setStr(TUTORIAL_03);
			break;

			case 4:
				enableRule(RULE_DRAIN_HP);
				ifaceTutorialText.setStr(TUTORIAL_04);
			break;

			case 5:
				enableRule(RULE_DRAIN_RESOURCES);
				ifaceTutorialText.setStr(TUTORIAL_05);
			break;

			case 6:
				enableRule(RULE_ENEMY_BUILD_TURRETS);
				ifaceTutorialText.setStr(TUTORIAL_06);
			break;

			case 7:
				enableRule(RULE_ENEMY_PREFER_CARROTS);
				ifaceTutorialText.setStr(TUTORIAL_07);
			break;

			case 8:
				enableRule(RULE_ENEMY_AVOID_TURRETS);
				ifaceTutorialText.setStr(TUTORIAL_08);
			break;

			case 9:
				enableRule(RULE_PLAYER_STOMPS_CARROTS);
				ifaceTutorialText.setStr(TUTORIAL_09);
			break;

			default:
			case 10:
				ifaceTutorialText.setStr(TUTORIAL_10);
				ifaceBtnTutorialNext->disable();
			break;
			}

		ifaceTutorialText.update();
		ifaceReceiverTutorial=0;
		}

	return false;
	}

void Tutorial::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	ifaceTutorialText.print(Orientation::GUI+Vector(128, 0));

	TowerDefense::print(tinterp);
	}

/*void Tutorial::clear()
	{
	TowerDefense::clear();
	}*/

/*void Tutorial::pause()
	{
	//
	}*/

/*void Tutorial::resume()
	{
	//
	}*/
