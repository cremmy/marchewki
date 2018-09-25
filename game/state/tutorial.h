/*
 * tutorial.h
 *
 *  Created on: 25 wrz 2018
 *      Author: crm
 */

#pragma once

#include "towerdefense.h"

namespace Game
	{
	namespace State
		{
		class Tutorial: public TowerDefense
			{
			protected:
				int tutorialStep;
				int ifaceReceiverTutorial;
				UI::Button* ifaceBtnTutorialNext;

				Engine::Graphics::UI::Text ifaceTutorialText;

			public:
				Tutorial();
				virtual ~Tutorial();

				virtual bool init(Engine::Core::Application *application) override;
				virtual bool update(float dt) override;
				virtual void print(float tinterp) override;
				//virtual void clear() override;
				//virtual void pause() override;
				//virtual void resume() override;

				virtual std::string getName() const override {return "tutorial";}
			};

		} /* namespace State */
	} /* namespace Game */
