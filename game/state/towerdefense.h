/*
 * towerdefense.h
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/base/applicationstate.h"
#include "../../engine/core/appeventlistener.h"
#include "../../engine/graphics/ui/text.h"
#include "../../engine/render/camera.h"

#include "../level.h"
#include "../turret_type.h"

namespace Game
	{
	class TPlayerBase;

	namespace UI
		{
		class Window;
		class Button;
		}

	namespace State
		{
		class TowerDefense: public Engine::Base::ApplicationState, public Engine::Core::AppEventListener
			{
			protected:
				enum class State
					{
					PLAYING,
					DEFEAT,
					VICTORY,
					} state;
				enum class Mode
					{
					NONE,
					BUILDING,
					SELECTED,
					} mode;
				enum class Zoom
					{
					ZOOM_25,
					ZOOM_50,
					ZOOM_100,
					ZOOM_200,
					};
				enum InterfaceAction
					{
					IFACE_BUILD_SINGLE=      0x1<<0,
					IFACE_BUILD_AOE=         0x1<<1,
					IFACE_BUILD_MINE=        0x1<<2,
					IFACE_BUILD_CARROT_FIELD=0x1<<3,
					IFACE_TURRET_UPGRADE=    0x1<<4,
					IFACE_TURRET_SELL=       0x1<<5,
					};

				void initModeNone();
				void initModeBuilding(TurretType turret);
				void initModeSelected(const Engine::Math::VectorI fposition);

				void updateModeBuilding(float dt);
				void updateModeSelected(float dt);

				void printModeBuilding(float tinterp);
				void printModeSelected(float tinterp);

				Level level;
				TPlayerBase* playerBase;

				Engine::Math::VectorI fposMouse;

				Engine::Render::Camera camera;

				int camTargetAngle;
				float camCurrentAngle;
				Zoom camTargetZoomName;
				float camTargetZoom;
				float camCurrentZoom;

				int ifaceReceiver;
				UI::Window* interface;
				UI::Button* ifaceBtnTSingle;
				UI::Button* ifaceBtnTAOE;
				UI::Button* ifaceBtnTMine;
				UI::Button* ifaceBtnTCarrot;
				UI::Button* ifaceBtnUpgrade;
				UI::Button* ifaceBtnSell;
				Engine::Graphics::SpritePtr ifaceResourcesIcon;
				Engine::Graphics::UI::Text ifaceResourcesText;

				struct
					{
					TurretType turret;
					Engine::Graphics::SpritePtr sprite;
					} modeBuildData;
				struct
					{
					Engine::Math::VectorI fposition;
					const Level::Field* field;
					} modeSelectedData;

				struct
					{
					float resources;
					float hp;
					int fieldWidth;
					int fieldHeight;
					} startSettings;

				std::list<Engine::Graphics::SpritePtr> spriteCache;

			public:
				// TODO parametry gry w konstruktorze (początkowe zasoby, reguły, wielkość planszy)
				TowerDefense(float resources, float hp, int fieldWidth, int fieldHeight);
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
