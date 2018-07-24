/*
 * towerdefense.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"

namespace Game
	{
	namespace State
		{
		class TowerDefense: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			public:
				TowerDefense();
				virtual ~TowerDefense();

				virtual bool init(Engine::Core::Application *application);
				virtual bool update(float dt);
				virtual void print(float tinterp);
				virtual bool isOverlay() const {return false;}
				virtual void clear();
				virtual void pause();
				virtual void resume();

				virtual std::string getName() const {return "game";}
			};
		} /* namespace State */
	} /* namespace Game */
