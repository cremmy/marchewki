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

//namespace Local
//	{
	Engine::Render::Camera cam;

	Level level;

	int camTargetAngle=1;
	float camCurrentAngle=camTargetAngle;
//	}

TowerDefense::TowerDefense()
	{
	//
	}

TowerDefense::~TowerDefense()
	{
	//
	}


bool TowerDefense::init(Engine::Core::Application *application)
	{
	//using namespace Local;
	using namespace Engine::Math;

	this->application=application;

	this->application->addListener(Engine::Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_KEY_UP, *this);
	this->application->addListener(Engine::Core::AppEvent::Type::MOUSE_WHEEL, *this);

	cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 1000.0f);
	//cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
	cam.lookAt(Vector(0, 0, 0), 45.0f, 30.0f, 512.0f);

	return level.init(8, 8);
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
				cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
				}
			else if(e.data.keyboard.key==SDLK_2)
				{
				cam.perspective(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
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

			cam.getRay(mx-e.data.mouse.x, my-e.data.mouse.y, raypos, raydir);
			const Vector POS_PREV=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

			cam.getRay(mx, my, raypos, raydir);
			const Vector POS_CUR=MathUtils::getPositionAtZ0ByRay(raypos, raydir);

			cam.move((POS_PREV-POS_CUR));
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==1)
			{
			Engine::Math::Vector raypos;
			Engine::Math::Vector raydir;

			cam.getRay(e.data.mouse.x, e.data.mouse.y, raypos, raydir);

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
			camTargetAngle+=e.data.mouse.y;
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

	if(camTargetAngle!=camCurrentAngle)
		{
		const float MOD=((camTargetAngle>camCurrentAngle)?dt:-dt) * 8.0f * (std::abs(camTargetAngle-camCurrentAngle));

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

		const Vector HIT=MathUtils::getPositionAtZ0ByRay(cam.getPosition(), cam.getForward());

		cam.lookAt(HIT, camCurrentAngle*45.0f, 30.0f, VectorLength(HIT-cam.getPosition()));
		}

	level.update(dt);

	return false;
	}

void TowerDefense::print(float tinterp)
	{
	//using namespace Local;

	Engine::Render::getInstance().setCamera(cam);

	level.print(tinterp);
	}

void TowerDefense::clear()
	{
	//using namespace Local;

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

