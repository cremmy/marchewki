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

using namespace Game;
using namespace Game::State;

const float CAMERA_ANGLE=45.0f;
const float CAMERA_ELEVATION=30.0f;
const float CAMERA_DISTANCE=1024.0f;

TowerDefense::TowerDefense(): mode(Mode::NONE), level(), selected(nullptr), camera(), camTargetAngle(1), camCurrentAngle(1)
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

	camera.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 2000.0f);
	//cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
	camera.lookAt(level.getFieldPosition(0u, 0u), CAMERA_ANGLE, CAMERA_ELEVATION, CAMERA_DISTANCE);

	if(!level.buildTurret(0, 0, TurretType::PLAYER_BASE))
		{
		LOG_ERROR("Nie udalo sie wstawic bazy gracza");
		return false;
		}
	if(!level.buildTurret(level.getWidth()-2, level.getHeight()-1, TurretType::ENEMY_SPAWNER))
		{
		LOG_ERROR("Nie udalo sie wstawic spawnera");
		return false;
		}

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
				camera.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 2000.0f);
				}
			else if(e.data.keyboard.key==SDLK_2)
				{
				camera.perspective(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 4000.0f);
				}
#endif
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE && (e.data.mouse.key&SDL_BUTTON(3)))
			{
			int mx;
			int my;

			SDL_GetMouseState(&mx, &my);

			Vector raypos;
			Vector raydir;

			camera.getRay(mx-e.data.mouse.x, my-e.data.mouse.y, raypos, raydir);
			const Vector POS_PREV=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

			camera.getRay(mx, my, raypos, raydir);
			const Vector POS_CUR=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

			camera.move((POS_PREV-POS_CUR));

			// Zapobieganie zbytniemu oddaleniu kamery
			const Vector LEVEL_MIN=level.getFieldPosition(0u, 0u);
			const Vector LEVEL_MAX=level.getFieldPosition(level.getWidth(), level.getHeight());
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
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==1)
			{
			Engine::Math::Vector raypos;
			Engine::Math::Vector raydir;

			camera.getRay(e.data.mouse.x, e.data.mouse.y, raypos, raydir);

			unsigned x=0u;
			unsigned y=0u;
			if(level.getFieldByRay(raypos, raydir, x, y))
				{
				LOG_DEBUG("HIT: %d, %d", x, y);
				}
			else
				{
				LOG_DEBUG("MISS");
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_WHEEL)
			{
			camTargetAngle+=e.data.mouse.y*2;
			/*if(camTargetAngle<0)
				{
				camTargetAngle+=8;
				}
			else if(camTargetAngle>=8)
				{
				camTargetAngle-=8;
				}*/
			}
		}

	//
	// Obracanie kamery
	//
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

		camera.lookAt(HIT, camCurrentAngle*CAMERA_ANGLE, CAMERA_ELEVATION, CAMERA_DISTANCE);
		}

	//
	// Sterowanie
	//
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

	level.update(dt);

	return false;
	}

void TowerDefense::updateModeBuilding(float dt)
	{
	//
	}

void TowerDefense::updateModeSelected(float dt)
	{
	//
	}


void TowerDefense::print(float tinterp)
	{
	Engine::Render::getInstance().setCamera(camera);

	level.print(tinterp);
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

