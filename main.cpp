/*
 * main.cpp
 *
 *  Created on: 20 lip 2018
 *      Author: crm
 */
// SDL_main
#include <SDL2/SDL.h>

#include "engine/debug/log.h"
#include "engine/core/application.h"
#include "engine/render/render.h"
#include "engine/sound/soundplayer.h"

#include "game/state/debug.h"

int main(int, char**)
	{
#ifdef BUILD_RELEASE
	Engine::Debug::Log::setOut(fopen("log.txt", "w"));
	Engine::Debug::Log::setLevel(Engine::Debug::Log::PRODUCTION);
#endif

	LOG_INFO("Start");
	LOG_INFO("Build %s %s", __DATE__, __TIME__);

	Engine::Core::Application app;

	if(!app.init())
		{
		return __LINE__;
		}

	if(!Engine::Render::getInstance().init(1024, 768, "Tower Defence Tycoon", "image/icon.png", Engine::Render::FullScreenMode::WINDOWED))
		{
		return __LINE__;
		}

	if(!Engine::Sound::getInstance().init(32))
		{
		return __LINE__;
		}

	if(app.pushState(new Game::State::Debug()))
		{
		app.run();
		}

	Engine::Sound::getInstance().clear();
	Engine::Render::getInstance().clear();
	app.clear();

	LOG_INFO("Stop");

	return 0;
	}


