/*
 * towerdefense.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "towerdefense.h"

#include <SDL2/SDL.h>

#include "../../engine/debug/log.h"
#include "../../engine/render/camera.h"
#include "../../engine/render/render.h"

#include "../level.h"
#include "../math_utils.h"

#include "../musicbox.h"

using namespace Game;
using namespace Game::State;

const float CAMERA_ANGLE=45.0f;
const float CAMERA_ELEVATION=30.0f;
const float CAMERA_DISTANCE=1024.0f;

MusicBox mb;

TowerDefense::TowerDefense(): mode(Mode::NONE), level(), playerBase(nullptr), camera(),
	camTargetAngle(1), camCurrentAngle(1),
	camTargetZoomName(Zoom::ZOOM_100), camTargetZoom(1.0f), camCurrentZoom(1.0f)
	{
	//
	}

TowerDefense::~TowerDefense()
	{
	//
	}


bool TowerDefense::init(Engine::Core::Application *application)
	{
	using namespace Engine::Math;

	this->application=application;

	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_UP, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_WHEEL, *this);

	if(!level.init(8, 6))
		{
		LOG_ERROR("Nie udalo sie zainicjowac poziomu");
		return false;
		}

	camera.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 4000.0f);
	//cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
	camera.lookAt(level.getFieldPosition({0, 0}), CAMERA_ANGLE, CAMERA_ELEVATION, CAMERA_DISTANCE);

	if(!level.buildTurret({0, 0}, TurretType::PLAYER_BASE))
		{
		LOG_ERROR("Nie udalo sie wstawic bazy gracza");
		return false;
		}
	if(!level.buildTurret({level.getWidth()-1, level.getHeight()-1}, TurretType::ENEMY_SPAWNER))
		{
		LOG_ERROR("Nie udalo sie wstawic spawnera");
		return false;
		}

	// Moja paranoja nie pozwala mi zaakceptować tego kodu
	// ...ale moja wiara w 'jakoś to będzie' pozwala mi zostawić modyfikacje na później
	//                                                                              ...czyli nigdy, o ile będzie działało
	playerBase=(TPlayerBase*)(((const Level&)level).getField({0, 0})->turret);

	// Preload spritów
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/collectible.xml"));
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/projectile.xml"));
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/unit_enemy_infantry.xml"));
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/unit_player_acolyte.xml"));
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/particle_red.xml"));
	spriteCache.push_back(Engine::Graphics::SpritePtr("sprite/particle_green.xml"));

	mb.setNotes(VIVALDI);

	return true;
	}

bool TowerDefense::update(float dt)
	{
	using namespace Engine::Math;

	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
#ifdef BUILD_DEBUG
			if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				application->stop();
				}
			if(e.data.keyboard.key==SDLK_1)
				{
				camera.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 4000.0f);
				}
			else if(e.data.keyboard.key==SDLK_2)
				{
				camera.perspective(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 4000.0f);
				}
#endif

			/*****************************************************************************/
			/**** Budowanie (klawiatura) *************************************************/
			/*****************************************************************************/
			if(e.data.keyboard.key==SDLK_q)
				{
				initModeBuilding(TurretType::PLAYER_UNIT_SINGLE_TARGET);
				}
			else if(e.data.keyboard.key==SDLK_w)
				{
				initModeBuilding(TurretType::PLAYER_UNIT_AREA_OF_EFFECT);
				}
			else if(e.data.keyboard.key==SDLK_e)
				{
				initModeBuilding(TurretType::PLAYER_UNIT_MINE);
				}
			else if(e.data.keyboard.key==SDLK_r)
				{
				initModeBuilding(TurretType::PLAYER_CARROT_FIELD);
				}
#ifdef BUILD_DEBUG
			else if(e.data.keyboard.key==SDLK_z)
				{
				if(level.getWidth()<=level.getHeight() && level.getWidth()<20)
					level.resizeIncreaseXByOne();
				else if(level.getHeight()<20)
					level.resizeIncreaseYByOne();
				}
#endif
			}
		/*****************************************************************************/
		/**** Ruch myszy *************************************************************/
		/*****************************************************************************/
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE)
			{
			int mx;
			int my;

			SDL_GetMouseState(&mx, &my);

			Vector raypos;
			Vector raydir;

			camera.getRay(mx, my, raypos, raydir);
			const Vector POS_CUR=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

			/*****************************************************************************/
			/**** Ruch kamery ************************************************************/
			/*****************************************************************************/
			if(e.data.mouse.key&SDL_BUTTON(3))
				{
				camera.getRay(mx-e.data.mouse.x, my-e.data.mouse.y, raypos, raydir);
				const Vector POS_PREV=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

				camera.move((POS_PREV-POS_CUR));

				// Zapobieganie zbytniemu oddaleniu kamery
				const Vector LEVEL_MIN=level.getFieldPosition({0, 0});
				const Vector LEVEL_MAX=level.getFieldPosition({level.getWidth()-1, level.getHeight()-1});
				const Vector CAMERA_CENTER=MathUtils::getPositionAtZ0ByRay(camera.getPosition(), camera.getForward());

				if(CAMERA_CENTER.x<LEVEL_MIN.x)
					{
					camera.move(Vector(LEVEL_MIN.x-CAMERA_CENTER.x, 0.0f));
					}
				else if(CAMERA_CENTER.x>LEVEL_MAX.x)
					{
					camera.move(Vector(LEVEL_MAX.x-CAMERA_CENTER.x, 0.0f));
					}

				if(CAMERA_CENTER.y<LEVEL_MIN.y)
					{
					camera.move(Vector(0.0f, LEVEL_MIN.y-CAMERA_CENTER.y));
					}
				else if(CAMERA_CENTER.y>LEVEL_MAX.y)
					{
					camera.move(Vector(0.0f, LEVEL_MAX.y-CAMERA_CENTER.y));
					}
				}
			}
		/*****************************************************************************/
		/**** Obrót kamery ***********************************************************/
		/*****************************************************************************/
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_WHEEL)
			{
			if(SDL_GetModState()&(KMOD_LCTRL|KMOD_RCTRL))
				{
				// Zoom
				if(std::abs(camTargetZoom-camCurrentZoom)>0.1f)
					{
					// NoOp
					}
				else if(e.data.mouse.y<0.0f)
					{
					switch(camTargetZoomName)
						{
						case Zoom::ZOOM_25:
							// NoOp
						break;

						case Zoom::ZOOM_50:
							camTargetZoomName=Zoom::ZOOM_25;
							camTargetZoom=0.25f;
						break;

						case Zoom::ZOOM_100:
							camTargetZoomName=Zoom::ZOOM_50;
							camTargetZoom=0.5f;
						break;

						case Zoom::ZOOM_200:
							camTargetZoomName=Zoom::ZOOM_100;
							camTargetZoom=1.0f;
						break;

						default:
							camTargetZoomName=Zoom::ZOOM_100;
							camTargetZoom=1.0f;
						break;
						}
					}
				else
					{
					switch(camTargetZoomName)
						{
						case Zoom::ZOOM_25:
							camTargetZoomName=Zoom::ZOOM_50;
							camTargetZoom=0.5f;
						break;

						case Zoom::ZOOM_50:
							camTargetZoomName=Zoom::ZOOM_100;
							camTargetZoom=1.0f;
						break;

						case Zoom::ZOOM_100:
							camTargetZoomName=Zoom::ZOOM_200;
							camTargetZoom=2.0f;
						break;

						case Zoom::ZOOM_200:
							// NoOp
						break;

						default:
							camTargetZoomName=Zoom::ZOOM_100;
							camTargetZoom=1.0f;
						break;
						}
					}
				}
			else
				{
				// Obrót
				camTargetAngle+=e.data.mouse.y*2;
				}
			}
		/*****************************************************************************/
		/**** Obsługa kliknięcia *****************************************************/
		/*****************************************************************************/
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==1)
			{
			// Czy kliknięto w któryś przycisk?
//			if(ui.click(e.data.mouse.x, e.data.mouse.y))
//				{
//				// nanana
//				}
//			else
				{
				Engine::Math::Vector raypos;
				Engine::Math::Vector raydir;

				camera.getRay(e.data.mouse.x, e.data.mouse.y, raypos, raydir);

				Engine::Math::VectorI fposition;
				if(level.getFieldByRay(raypos, raydir, fposition))
					{
					LOG_DEBUG("HIT: %d, %d [click %d %d]", fposition.x, fposition.y, e.data.mouse.x, e.data.mouse.y);

					switch(mode)
						{
						case Mode::BUILDING:
							if(level.buildTurret(fposition, modeBuildData.turret))
								{
								initModeNone();
								}
						break;

						case Mode::SELECTED:
							initModeSelected(fposition);
						break;

						default:
						case Mode::NONE:
							initModeSelected(fposition);
						break;
						}
					}
				else
					{
					initModeNone();
					}
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==3)
			{
			initModeNone();
			}
		}

	/*****************************************************************************/
	/**** Faktyczny obrót kamery *************************************************/
	/*****************************************************************************/
	if(camTargetAngle!=camCurrentAngle)
		{
		const float MOD=((camTargetAngle>camCurrentAngle)?1.0f:-1.0f) *
			(1.0f*dt +
			(8.0f*dt*(std::abs(camTargetAngle-camCurrentAngle))));

		if(std::abs(camTargetAngle-camCurrentAngle)>MOD)
			{
			camCurrentAngle+=MOD;
			}
		else
			{
			camTargetAngle=camTargetAngle%8;

			camCurrentAngle=camTargetAngle;
			}

		Vector raypos;
		Vector raydir;

		const Vector HIT=MathUtils::getPositionAtZ0ByRay(camera.getPosition(), camera.getForward());

		camera.lookAt(HIT, camCurrentAngle*CAMERA_ANGLE, CAMERA_ELEVATION, CAMERA_DISTANCE/camCurrentZoom);
		}

	/*****************************************************************************/
	/**** Faktyczne skalowanie kamery ********************************************/
	/*****************************************************************************/
	if(camTargetZoom!=camCurrentZoom)
		{
		const float MOD=((camTargetZoom>camCurrentZoom)?1.0f:-1.0f) *
			(1.0f*dt +
			(8.0f*dt*(std::abs(camTargetZoom-camCurrentZoom))));

		if(std::abs(camTargetZoom-camCurrentZoom)>MOD)
			{
			camCurrentZoom+=MOD;
			}
		else
			{
			camCurrentZoom=camTargetZoom;
			}

		camera.setScale(camCurrentZoom);
		camCurrentAngle-=0.01f;

		LOG_DEBUG("[%f:%f][mod %f]", camCurrentZoom, camTargetZoom, MOD);
		}

	/*****************************************************************************/
	/**** Sterowanie *************************************************************/
	/*****************************************************************************/
	level.update(dt);

	switch(mode)
		{
		case Mode::BUILDING:
			updateModeBuilding(dt);
		break;

		case Mode::SELECTED:
			updateModeSelected(dt);
		break;

		default:
		case Mode::NONE:
			//
		break;
		}

	//mb.setCreepiness(playerBase->getUpgrade()/(float)(playerBase->getMaxUpgrade()-1));
	//mb.setCreepiness(1.0f);
	//mb.update(dt);

	return false;
	}


void TowerDefense::updateModeBuilding(float dt)
	{
	//
	}

void TowerDefense::updateModeSelected(float dt)
	{
	level.setFieldHighlight(modeSelectedData.fposition);
	}


void TowerDefense::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	Render::getInstance().setRenderMode(Engine::Render::RenderMode::NORMAL);
	Render::getInstance().setCamera(camera);

	level.print(tinterp);

	switch(mode)
		{
		case Mode::BUILDING:
			printModeBuilding(tinterp);
		break;

		case Mode::SELECTED:
			printModeSelected(tinterp);
		break;

		default:
		case Mode::NONE:
			//
		break;
		}

	//Render::getInstance().setRenderMode(Engine::Render::RenderMode::GUI);
	}


void TowerDefense::printModeBuilding(float tinterp)
	{
	//
	}

void TowerDefense::printModeSelected(float tinterp)
	{
	//
	}


void TowerDefense::clear()
	{
	level.clear();
	}

void TowerDefense::pause()
	{
	//
	}

void TowerDefense::resume()
	{
	//
	}


void TowerDefense::initModeNone()
	{
	LOG_DEBUG("Mode: NONE");

	mode=Mode::NONE;
	}

void TowerDefense::initModeBuilding(TurretType turret)
	{
	LOG_DEBUG("Mode: BUILDING; Turret: %d", turret);

	mode=Mode::BUILDING;
	modeBuildData.turret=turret;
	}

void TowerDefense::initModeSelected(const Engine::Math::VectorI fposition)
	{
	LOG_DEBUG("Mode: SELECTED; Field: %d,%d", fposition.x, fposition.y);

	mode=Mode::SELECTED;
	modeSelectedData.fposition=fposition;
	modeSelectedData.field=((const Level*)&level)->getField(fposition);

	if(!modeSelectedData.field /*|| !modeSelectedData.field->turret*/)
		{
		return initModeNone();
		}
	}
