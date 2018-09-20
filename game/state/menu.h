/*
 * menu.h
 *
 *  Created on: 20 wrz 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"

namespace Game
	{
	namespace State
		{
		class Menu: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				//

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
