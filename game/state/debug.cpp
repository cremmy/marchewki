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

//#include "../level.h"

using namespace Game;
using namespace Game::State;

namespace Local
	{
	Engine::Render::Camera cam;

	Engine::Graphics::ImagePtr grid;

	Engine::Graphics::SpritePtr sprite;
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

	//cam.ortho(Engine::Render::getInstance().getWindowWidth(), Engine::Render::getInstance().getWindowHeight(), 1.0f, 1000.0f);
	cam.perspective(Render::getInstance().getWindowWidth(), Render::getInstance().getWindowHeight(), 0.1);
	//cam.lookAt(Engine::Math::Vector(0, 0, 0), Engine::Math::Vector(0, 0, 500), Engine::Math::Vector(0, 1, 0));
	cam.lookAt(Vector(0, 0, 24), Vector(-64, -64, 64));

	if(!(grid=Graphics::ImagePtr("image/grid.png")))
		return false;

	if(!(sprite=Graphics::SpritePtr("sprite/golab.xml")))
		return false;

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
		}

	sprite.update(dt);

	//lvl->update(dt);

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
	Engine::Render::getInstance().draw(Orientation::FLAT_XY+Vector(-grid->getW()*0.5f, grid->getH()*0.5f, -0.125f), grid);

	Engine::Render::getInstance().draw(cam.getBillboard(Vector(0, 0, 32)), sprite);
	//Engine::Render::getInstance().draw(Orientation::FLAT_XZ+Vector(0, 0, 32), sprite);

	//return false; // Tak, wyświetlaj stany poniżej
	}

void Debug::clear()
	{
	using namespace Local;

	LOG_DEBUG("State.Debug.clear");

	grid=nullptr;
	sprite=nullptr;
	}

void Debug::pause()
	{
	LOG_DEBUG("State.Debug.pause");
	}

void Debug::resume()
	{
	LOG_DEBUG("State.Debug.resume");
	}

