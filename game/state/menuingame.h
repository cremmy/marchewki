/*
 * menuingame.h
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

		class MenuIngame: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
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

				virtual std::string getName() const {return"menu_ingame";}
			};

		} /* namespace State */
	} /* namespace Game */
