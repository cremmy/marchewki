/*
 * menu.h
 *
 *  Created on: 20 wrz 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"

#include "../ui/button.h"
#include "../ui/checkbutton.h"
#include "../ui/window.h"

namespace Game
	{
	namespace State
		{
		class Menu: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				enum InterfaceAction
					{
					IFACE_GOTO_NEW_GAME=     0x1<<0,
					IFACE_START_TUTORIAL=    0x1<<1,
					IFACE_GOTO_OPTIONS=      0x1<<2,
					IFACE_GOTO_HELP=         0x1<<3,
					IFACE_EXIT=              0x1<<4,
					IFACE_BACK=              0x1<<5,

					IFACE_START_SANDBOX=     0x1<<8,
					IFACE_START_EASY=        0x1<<9,
					IFACE_START_MEDIUM=      0x1<<10,
					IFACE_START_HARD=        0x1<<11,
					IFACE_GOTO_CUSTOM=       0x1<<12,

					IFACE_START_CUSTOM=      0x1<<16,
					};

				UI::Window* wndMain;
				UI::Button* btnMainNewGame;
				UI::Button* btnMainTutorial;
				UI::Button* btnMainOptions;
				UI::Button* btnMainHelp;
				UI::Button* btnMainExit;

				UI::Window* wndNewGame;
				UI::Button* btnNewGameSandbox;
				UI::Button* btnNewGameEasy;
				UI::Button* btnNewGameMedium;
				UI::Button* btnNewGameHard;
				UI::Button* btnNewGameCustom;
				UI::Button* btnNewGameBack;

				UI::Window* wndCustom;
				UI::CheckButton* chkCustomRuleBuildCost;
				UI::CheckButton* chkCustomRuleEnemySpawn;
				UI::CheckButton* chkCustomRuleDrainHP;
				UI::CheckButton* chkCustomRuleDrainRes;
				UI::CheckButton* chkCustomRuleEnemySpread;
				UI::CheckButton* chkCustomRulePreferCarrots;
				UI::CheckButton* chkCustomRuleAvoidTurrets;
				UI::CheckButton* chkCustomRulePlayerStomp;
				UI::Button* btnCustomStart;
				UI::Button* btnCustomBack;

				UI::Window* wndOptions;
				UI::CheckButton* chkOptionsSounds;
				UI::CheckButton* chkOptionsMusic;
				UI::Button* btnOptionsBack;

				UI::Window* wndHelp;
				UI::Button* btnHelpBack;

				UI::Window* currentWindow;
				int receiver;

			public:
				Menu();
				virtual ~Menu();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual void print(float tinterp);
				virtual bool isOverlay() const {return false;}
				virtual void clear();
				virtual void pause();
				virtual void resume();

				virtual std::string getName() const {return "menu";}
			};
		}
	} /* namespace Game */
