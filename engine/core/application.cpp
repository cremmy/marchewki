/*
 * application.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "application.h"

#include <SDL2/SDL.h>

#include "../base/applicationstate.h"

#include "../debug/log.h"
#include "../render/render.h"
#include "appeventlistener.h"

using namespace Engine::Core;

const unsigned UPDATE_FPS=60u;

Application::Application(): running(false), updatetime(0.0f), printtime(0.0f), fps(0.0f)
	{
	states.reserve(8);
	}

Application::~Application()
	{
	states.clear();
	}


bool Application::init()
	{
	LOG_INFO("Application.init: Startowanie SDLa...");

	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	LOG_INFO("Application.init: SDL: %d.%d.%d (link %d.%d.%d)",
			compiled.major, compiled.minor, compiled.patch,
			linked.major, linked.minor, linked.patch);
	LOG_INFO("Application.init: Platforma: %s", SDL_GetPlatform());

	LOG_INFO("Application.init: Info:");
	LOG_INFO("Application.init:     Core : %d", SDL_GetCPUCount());
	LOG_INFO("Application.init:     Cache: %d", SDL_GetCPUCacheLineSize());
	LOG_INFO("Application.init:     RAM  : %d", SDL_GetSystemRAM());
	LOG_INFO("Application.init:     SIMD : %s%s%s%s%s%s%s%s",
			SDL_HasMMX()?"MMX ":"",
			SDL_HasSSE()?"SSE ":"",
			SDL_HasSSE2()?"SSE2 ":"",
			SDL_HasSSE3()?"SSE3 ":"",
			SDL_HasSSE41()?"SSE4.1 ":"",
			SDL_HasSSE42()?"SSE4.2 ":"",
			SDL_HasAVX()?"AVX ":"",
			SDL_HasAVX2()?"AVX2 ":"");

	// Wylaczanie nazywania watkow - GDB tego nie lubi i sie crashuje >;
	SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)!=0)
	//if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
		{
		LOG_ERROR("Application.init: Nie udalo sie wystartowac SDLa: %s", SDL_GetError());
		return false;
		}

	LOG_SUCCESS("Application.init: Wystartowano SDLa");

	return true;
	}

void Application::run()
	{
	SDL_Event event;

	running=true;

	unsigned accumulator=0u;

	int tmpaccumulator=0;
	int tmpupdatecount=0;
	int tmpupdatetime=0;
	int tmpprintcount=0;
	int tmpprinttime=0;

	LOG_INFO("Application.run: Uruchamianie petli glownej");

	while(running)
		{
		const unsigned LOOP_TIME_START=SDL_GetTicks();

		while(SDL_PollEvent(&event))
			{
			switch(event.type)
				{
				case SDL_QUIT:
					stop();
				break;

				case SDL_KEYDOWN:
#ifdef BUILD_DEBUG
					//if(event.key.keysym.sym==SDLK_ESCAPE)
					//	stop();
#endif
					if(event.key.keysym.sym==SDLK_F4 && event.key.keysym.mod==KMOD_LALT)
						stop();

					if(event.key.keysym.sym==SDLK_RETURN && event.key.keysym.mod==KMOD_LALT)
						Render::getInstance().toggleFullscreen();

					pushEvent(AppEvent().initKeyDown(event.key.keysym.sym, event.key.keysym.mod));
				break;

				case SDL_KEYUP:
					pushEvent(AppEvent().initKeyUp(event.key.keysym.sym, event.key.keysym.mod));
				break;

				case SDL_MOUSEMOTION:
					pushEvent(AppEvent().initMouseMove(event.motion.xrel, event.motion.yrel, event.motion.state));
				break;

				case SDL_MOUSEBUTTONDOWN:
					pushEvent(AppEvent().initMouseKeyDown(event.button.x, event.button.y, event.button.button));
				break;

				case SDL_MOUSEBUTTONUP:
					pushEvent(AppEvent().initMouseKeyUp(event.button.x, event.button.y, event.button.button));
				break;

				case SDL_MOUSEWHEEL:
					pushEvent(AppEvent().initMouseWheel(event.wheel.y));
				break;

				case SDL_WINDOWEVENT:
					switch(event.window.event)
						{
						case SDL_WINDOWEVENT_MINIMIZED:
							pushEvent(AppEvent().initWindowHide());
						break;

						case SDL_WINDOWEVENT_RESTORED:
							pushEvent(AppEvent().initWindowShow());
						break;

						default:
							//
						break;
						}
				break;

				default:
					//
				break;
				}

			/*auto it=listeners.find(event.type);
			if(it!=listeners.end())
				{
				EventListener* listener=it->second;

				listener->sdlevent(sdlevent);
				}*/
			}

		// Update
		const unsigned LOOP_UPDATE_DELAY=1000/UPDATE_FPS;
		const float LOOP_UPDATE_DT=LOOP_UPDATE_DELAY/1000.0f;

		for(unsigned i=0u; accumulator>=LOOP_UPDATE_DELAY; ++i)
			{
			tmpupdatecount++;

			accumulator-=LOOP_UPDATE_DELAY;

			for(auto it=states.rbegin(); it!=states.rend(); ++it)
				{
				if(!(*it)->update(LOOP_UPDATE_DT))
					break;
				}

			if(i>=4u)
				{
				// Za duzo update'ow w kolejce, obcinanie
				accumulator=accumulator%LOOP_UPDATE_DELAY;
				}

			//LOG_DEBUG("loop %u -> update %u", accumulator, i);
			}

		tmpupdatetime+=SDL_GetTicks()-LOOP_TIME_START;

		// Print
		const unsigned PRINT_TIME_START=SDL_GetTicks();
		tmpprintcount++;

		/*for(auto it=states.rbegin(); it!=states.rend(); ++it)
			{
			if(!(*it)->print((accumulator%LOOP_UPDATE_DELAY)/1000.0F))
				break;
			}*/

		int count=0;
		for(auto it=states.rbegin(); it!=states.rend(); ++it)
			{
			if(!(*it)->isOverlay())
				break;

			++count;
			}

		for(auto it=states.begin()+states.size()-1-count; it!=states.end(); ++it)
			{
			(*it)->print((accumulator%LOOP_UPDATE_DELAY)/1000.0F);
			}

		tmpprinttime+=SDL_GetTicks()-PRINT_TIME_START;

		//LOG_DEBUG("loop %u [%f]", accumulator, (accumulator%LOOP_UPDATE_DELAY)/(float)LOOP_UPDATE_DELAY);

		// Flip
		Render::getInstance().update();

		// Inkrementacja akumulatora
		const unsigned LOOP_ACCUMULATOR_STEP=SDL_GetTicks()-LOOP_TIME_START;

		/*if(LOOP_ACCUMULATOR_STEP<1u)
			{
			SDL_Delay(1u);
			accumulator+=1u;
			}*/

		accumulator+=LOOP_ACCUMULATOR_STEP;

		tmpaccumulator+=SDL_GetTicks()-LOOP_TIME_START;

		if(tmpaccumulator>1000)
			{
			tmpaccumulator-=1000;

			updatetime=tmpupdatetime/(float)tmpupdatecount;
			printtime=tmpprinttime/(float)tmpprintcount;
			fps=tmpprintcount;

			LOG_DEBUG("[upd %.2f][prt %.2f][fps %.2f]", updatetime, printtime, fps);

			tmpupdatecount=0;
			tmpupdatetime=0;
			tmpprintcount=0;
			tmpprinttime=0;
			}
		}

	LOG_WARNING("Application.run: Opuszczono petle glowna");
	}

void Application::clear()
	{
	listeners.clear();

	LOG_DEBUG("Application.run: Zrzucanie stanow ze stosu...");
	while(popState());
	LOG_DEBUG("Application.run: Zrzucono stany ze stosu");

	LOG_INFO("Application.run: Zamykanie SDLa...");
	SDL_Quit();
	LOG_SUCCESS("Application.run: Zamknieto SDLa");
	}


bool Application::pushState(Base::ApplicationState* state)
	{
	LOG_INFO("Application.pushState: Wrzucanie stanu \"%s\" na stos (aktualnie %u stanow)", state->getName().c_str(), states.size());

	if(states.size()>0u)
		{
		LOG_INFO("Application.pushState: Pauzowanie poprzedniego stanu, \"%s\"...", states.back()->getName().c_str());

		states.back()->pause();
		}

	LOG_INFO("Application.pushState: Inicjalizacja...");

	if(!state->init(this))
		{
		LOG_ERROR("Application.pushState: Inicjalizacja nie powiodla sie, czyszczenie nowego stanu");

		state->clear();
		delete state;

		LOG_WARNING("Application.pushState: Przywracanie poprzedniego stanu");

		if(!states.empty())
			{
			states.back()->resume();
			}
		else
			{
			LOG_WARNING("Application.pushState: Brak stanow na stosie, zamykanie programu");
			stop();
			}

		return false;
		}

	states.push_back(state);

	LOG_SUCCESS("Application.pushState: Nowy stan wrzucony na stos");

	return true;
	}

bool Application::popState()
	{
	if(states.empty())
		return false;

	LOG_INFO("Application.popState: Zdejmowanie stanu \"%s\" ze stosu (aktualnie %u stanow)", states.back()->getName().c_str(), states.size());

	LOG_INFO("Application.popState: Czyszczenie...");
	states.back()->clear();

	LOG_INFO("Application.popState: Kasowanie...");
	delete states.back();
	states.pop_back();

	if(states.size()>0u)
		{
		LOG_INFO("Application.popState: Wznawianie poprzedniego stanu, \"%s\"...", states.back()->getName().c_str());

		states.back()->resume();
		}

	LOG_SUCCESS("Application.popState: Stan zdjety ze stosu");

	return true;
	}


void Application::addListener(AppEvent::Type type, AppEventListener& listener)
	{
#ifdef BUILD_DEBUG
	if(listeners.find(type)!=listeners.end())
		{
		for(auto* l: listeners[type])
			{
			assert(l!=&listener);
			}
		}
#endif

	listeners[type].push_back(&listener);
	}

void Application::removeListener(AppEventListener& listener)
	{
	for(auto itt=listeners.begin(); itt!=listeners.end(); ++itt)
		{
		for(auto itl=itt->second.begin(); itl!=itt->second.end(); ++itl)
			{
			if(*itl!=&listener)
				continue;

			LOG_DEBUG("Application.removeListener: Kasowanie listenera typu %d", (int)itt->first);
			itt->second.erase(itl--);
			}
		}
	}


void Application::pushEvent(const AppEvent& e)
	{
	auto itt=listeners.find(e.getType());

	if(itt==listeners.end())
		return;

	for(auto listener: itt->second)
		{
		listener->pushEvent(e);
		}
	}


void Application::stop()
	{
	running=false;
	}


void Application::setGrabMouse(bool s)
	{
	if(s)
		{
		if(SDL_GetRelativeMouseMode()!=SDL_TRUE)
			{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}
	else
		{
		if(SDL_GetRelativeMouseMode()!=SDL_FALSE)
			{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
	}
