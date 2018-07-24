/*
 * debug.cpp
 *
 *  Created on: 19 cze 2017
 *      Author: crm
 */

#include "debug.h"

#include "../../engine/debug/log.h"
#include "../../engine/render/camera.h"
#include "../../engine/render/render.h"

#include "../../engine/graphics/imageptr.h"
#include "../../engine/graphics/spriteptr.h"

#include "../level.h"

using namespace Game;
using namespace Game::State;

namespace Local
	{
	Engine::Render::Camera cam;

	Engine::Graphics::SpritePtr img;

	Level level;
	}

Debug::Debug(): ApplicationState()
	{
	//
	}

Debug::~Debug()
	{
	//
	}

bool Debug::init(Engine::Core::Application *application)
	{
	using namespace Engine;
	using namespace Engine::Math;
	using namespace Local;

	LOG_DEBUG("State.Debug.init");

	this->application=application;
	this->application->addListener(Core::AppEvent::Type::KEY_DOWN, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_MOVE, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_KEY_DOWN, *this);
	this->application->addListener(Core::AppEvent::Type::MOUSE_KEY_UP, *this);

	cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 2048.0f);
	//cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
	cam.lookAt(Vector(4*70, 3*70, 24), 45.0f, 30.0f, 512.0f);

	img=Engine::Graphics::SpritePtr("sprite/golab.xml");

	/*if(!(grid=Graphics::ImagePtr("image/grid.png")))
		return false;

	if(!(sprite=Graphics::SpritePtr("sprite/golab.xml")))
		return false;*/

	level.init(8, 6);

	return true;
	}

bool Debug::update(float dt)
	{
	using namespace Local;

	const Uint8* key=SDL_GetKeyboardState(nullptr);

	const float CAM_SPEED=32.0f;
	const float CAM_SPEED_MOD=8.0f;

	if(key[SDL_SCANCODE_W]) cam.moveForward( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_S]) cam.moveForward(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_D]) cam.moveRight( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_A]) cam.moveRight(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_SPACE]) cam.moveUp( dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));
	if(key[SDL_SCANCODE_LCTRL]) cam.moveUp(-dt*CAM_SPEED*(1.0f+CAM_SPEED_MOD*key[SDL_SCANCODE_LSHIFT]));

	Engine::Core::AppEvent e;

	while(popEvent(e)) // Engine::Core::EventListener::popEvent(e)
		{
		if(e.getType()==Engine::Core::AppEvent::Type::KEY_DOWN)
			{
			if(e.data.keyboard.key==SDLK_1)
				{
				cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
				}
			else if(e.data.keyboard.key==SDLK_2)
				{
				cam.perspective(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight());
				}
			else if(e.data.keyboard.key==SDLK_3)
				{
				cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
				}
			else if(e.data.keyboard.key==SDLK_ESCAPE)
				{
				application->stop();
				}
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_MOVE && (e.data.mouse.key&SDL_BUTTON(3)))
			{
			cam.rotateZ(e.data.mouse.x*0.2f);
			cam.rotateRight(e.data.mouse.y*0.2f);
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_DOWN && e.data.mouse.key==3)
			{
			this->application->setGrabMouse(true);
			}
		else if(e.getType()==Engine::Core::AppEvent::Type::MOUSE_KEY_UP && e.data.mouse.key==3)
			{
			this->application->setGrabMouse(false);
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
		}

	//sprite.update(dt);

	level.update(dt);

	return false; // nie, nie aktualizuj stanów poniżej
	}

void Debug::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Local;

	Engine::Render::getInstance().setCamera(cam);

	Engine::Render::getInstance().drawLine(Vector(0, 0, 0), Vector(2, 0, 0), Vector(1, 0, 0, 1));
	Engine::Render::getInstance().drawLine(Vector(0, 0, 0), Vector(0, 2, 0), Vector(0, 1, 0, 1));
	Engine::Render::getInstance().drawLine(Vector(0, 0, 0), Vector(0, 0, 2), Vector(0, 0, 1, 1));
	//Engine::Render::getInstance().draw(Orientation::FLAT_XY+Vector(-grid->getW()*0.5f, grid->getH()*0.5f, -0.125f), grid);

	//Engine::Render::getInstance().draw(cam.getBillboard(Vector(0, 0, 32)), sprite);
	//Engine::Render::getInstance().draw(Orientation::FLAT_XZ+Vector(0, 0, 32), sprite);

	level.print(tinterp);

	Engine::Render::getInstance().draw(cam.getBillboard(Vector(2*70+35, 3*70+35, 0)), img);

	//return false; // Tak, wyświetlaj stany poniżej
	}

void Debug::clear()
	{
	using namespace Local;

	LOG_DEBUG("State.Debug.clear");

	level.clear();
	img=nullptr;
	}

void Debug::pause()
	{
	LOG_DEBUG("State.Debug.pause");
	}

void Debug::resume()
	{
	LOG_DEBUG("State.Debug.resume");
	}

