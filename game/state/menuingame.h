/*
 * menuingame.h
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
		class MenuIngame: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				enum InterfaceAction
					{
					IFACE_RESUME_GAME=       0x1<<0,
					IFACE_GOTO_HELP=         0x1<<1,
					IFACE_GOTO_RULES=        0x1<<2,
					IFACE_GOTO_MENU=         0x1<<3,
					IFACE_GOTO_OS=           0x1<<4,
					IFACE_BACK=              0x1<<8,
					};

				UI::Window* wndMain;
				UI::Button* btnMainResume;
				UI::Button* btnMainHelp;
				UI::Button* btnMainRules;
				UI::Button* btnMainExitMenu;
				UI::Button* btnMainExitOS;

				UI::Window* wndRules;
				UI::CheckButton* chkRulesBuildCost;
				UI::CheckButton* chkRulesEnemySpawn;
				UI::CheckButton* chkRulesDrainHP;
				UI::CheckButton* chkRulesDrainRes;
				UI::CheckButton* chkRulesEnemySpread;
				UI::CheckButton* chkRulesPreferCarrots;
				UI::CheckButton* chkRulesAvoidTurrets;
				UI::CheckButton* chkRulesPlayerStomp;
				UI::Button* btnCustomResume;
				UI::Button* btnCustomBack;

				UI::Window* wndHelp;
				UI::Button* btnHelpBack;

				UI::Window* currentWindow;
				int receiver;

			public:
				MenuIngame();
				virtual ~MenuIngame();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual void print(float tinterp);
				virtual bool isOverlay() const {return true;}
				virtual void clear();
				virtual void pause();
				virtual void resume();

				virtual std::string getName() const {return "menu_ingame";}
			};

		} /* namespace State */
	} /* namespace Game */
