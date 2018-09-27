/*
 * help.h
 *
 *  Created on: 25 wrz 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"
#include "../../engine/graphics/ui/text.h"

#include "../ui/button.h"
#include "../ui/checkbutton.h"
#include "../ui/valueselector.h"
#include "../ui/window.h"

namespace Game
	{
	namespace State
		{
		class Help: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				enum InterfaceAction
					{
					IFACE_BACK=          0x1<<0,
					IFACE_BASICS=        0x1<<1,
					IFACE_TURRETS=       0x1<<2,
					IFACE_TURRET_SINGLE =0x1<<3,
					IFACE_TURRET_AOE    =0x1<<4,
					IFACE_TURRET_MINE   =0x1<<5,
					IFACE_TURRET_CARROTS=0x1<<6,
					IFACE_TURRET_BASE   =0x1<<7,
					IFACE_TURRET_SPAWNER=0x1<<8,
					IFACE_ENEMIES=       0x1<<9,
					IFACE_RULES=         0x1<<10,
					};
				enum class Menu
					{
					MAIN,
					BASICS,
					TURRETS,
					TURRET_SINGLE,
					TURRET_AOE,
					TURRET_MINE,
					TURRET_CARROTS,
					TURRET_BASE,
					TURRET_SPAWNER,
					ENEMIES,
					RULES,
					};

				Menu currentMenu;
				int receiver;

				UI::Window* window;
				Engine::Graphics::UI::Text text;

				bool initMenu(Menu menu);

			public:
				Help();
				virtual ~Help();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual void print(float tinterp);
				virtual bool isOverlay() const {return false;}
				virtual void clear();
				virtual void pause();
				virtual void resume();

				virtual std::string getName() const {return "help";}
			};
		} /* namespace State */
	} /* namespace Game */
