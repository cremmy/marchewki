/*
 * debug.h
 *
 *  Created on: 19 cze 2017
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"

#include "../../engine/core/appeventlistener.h"

namespace Game
	{
	namespace State
		{
		class Debug: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			public:
				Debug();
				virtual ~Debug();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual void print(float tinterp);
				virtual bool isOverlay() const {return false;}
				virtual void clear();
				virtual void pause();
				virtual void resume();

				//virtual int getId() const {return 0xDEB0C;};
				virtual std::string getName() const {return "debug";}
			};

		} /* namespace State */
	} /* namespace Game */
