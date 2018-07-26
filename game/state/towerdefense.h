/*
 * towerdefense.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"
#include "../../engine/render/camera.h"

#include "../level.h"
#include "../turret_type.h"

namespace Game
	{
	namespace State
		{
		class TowerDefense: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				enum class Mode
					{
					NONE,
					BUILDING,
					SELECTED,
					} mode;

				void initModeBuilding(TurretType turret);
				void initModeSelected(int x, int y);

				void updateModeBuilding(float dt);
				void updateModeSelected(float dt);

				Level level;
				Level::Field* selected;

				Engine::Render::Camera camera;

				int camTargetAngle;
				float camCurrentAngle;

				struct
					{
					TurretType turret;
					} modeBuildData;

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
