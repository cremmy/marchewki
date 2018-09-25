/*
 * main.cpp
 *
 *  Created on: 20 lip 2018
 *      Author: crm
 */
/* Opis gry:
 *  - Siatka pól - neutralne, gracza i przeciwnika
 *  - Utrzymanie każdego pola kosztuje gracza zasoby
 *  - Neutralne pola co jakiś czas zmieniają się w pola przeciwnika (przeciwnik się powoli rozrasta?)
 *  - Pola przeciwnika falami emitują jednostki
 *  - Im więcej pól zajęte przez przeciwnika, tym silniejsze jednostki idą
 *
 *  - Gracz może zbudować wieżę, która zajmuje sąsiednie pola (o ile nie są wrogie)
 *  - Gracz może zbudowć farmę marchewek, która nie zużywa zasobów i nie zajmuje sąsiednich pól
 *
 */
/* Pomysły:
 *  ~ Muzyka z 'pozytywki' - pojedyncze sample odtwarzane według pewnego wzoru, im wyższy poziom bazy tym większa część sampli przełączana jest na przesterowane/fałszywe
 *  ~ Jeśli wieża obok jednostki wroga jest polem marchewkowym, to idź do niej. Po wejściu na to pole, HP gracza spada o 1.
 */
/* TODO:
 *  + Paski zdrowia pod osłabionymi jednostkami wroga
 *  + Świeczki informujące o poziomie wieży
 *  + Pociski wylatujące z wież?
 *  + Wieże:
 *    + Pojedyncza
 *    + Obszarowa
 *    + Mina
 *    + Marchewkowe pole
 * + Zbierajki wypadające z pokonanych wrogów
 * + Jednostka gracza zbierająca zbierajki
 * + Niszczenie wież wroga/zajmowanie pól wroga
 * + Tworzenie nowych wież wroga (tworzenie pierwszej gdy brak jakichkolwiek)
 * + Ograniczenie dostępnego terenu pod budowę wież przez gracza
 * + Obsługa zasobów
 * + Obsługa HP gracza
 *    + Utrata HP po wejściu wroga do bazy
 *    + Powolna utrata HP gdy zasoby==0
 *    + Koniec gry gdy HP=0
 * + Wygranie gry
 * + Interfejs graficzny
 * + Obsługa reguł gry
 * + Zliczanie wież w pobliżu ścieżki
 * + Ruch kamery strzałkami
 * ~ Nie odznaczać przy ruchu kamerą?
 * + Miny generują zasoby - więcej gdy na cooldownie?
 * + Okienko z opisem wieży (hover nad przyciskiem)
 * ~ Menu główne
 * + Hover nawet na wyłączonych
 * + Alternatywnie: koszt budowania na przyciskach
 * - Tutorial
 * - Grafika: ekran wczytywania
 * - Grafika: tło menu
 * - State: Help?
 */
/* FIXME:
 * + Animacja spawnerów czasem nie wraca na idle
 * ~ Spawnery wracają do stanu 'spawning'?
 */
/* Balans:
 *  + Za duży drain z pól na wczesnych etapach gry
 *  + Jedna wieża nie radzi sobie z początkowymi falami
 *  ? Drain z pól zalezny od procentowego zajęcia planszy?
 *  + Drain z pól nie powinien być zależny od aktualnych zasobów
 */
// SDL_main
#include <cstdio>
#include <ctime>
#include <SDL2/SDL.h>

#include "engine/debug/log.h"
#include "engine/core/application.h"
#include "engine/render/render.h"
#include "engine/sound/soundplayer.h"

#include "game/state/debug.h"
#include "game/state/towerdefense.h"
#include "game/state/menu.h"

#include "game/ui/panel.h"

int main(int, char**)
	{
#ifdef BUILD_RELEASE
	//Engine::Debug::Log::setOut(fopen("log.txt", "w"));
	Engine::Debug::Log::setLevel(Engine::Debug::Log::PRODUCTION);
#endif

	LOG_INFO("Start");
	LOG_INFO("Build %s %s", __DATE__, __TIME__);

	srand(time(nullptr));

	Engine::Core::Application app;

	if(!app.init())
		{
		return __LINE__;
		}

#ifdef BUILD_RELEASE
	if(!Engine::Render::getInstance().init(1024, 768, "Marchewki", "image/icon.png", Engine::Render::FullScreenMode::WINDOWED_FULLSCREEN))
#else
	if(!Engine::Render::getInstance().init(1024, 768, "Marchewki", "image/icon.png", Engine::Render::FullScreenMode::WINDOWED))
#endif
		{
		return __LINE__;
		}

	if(!Engine::Sound::getInstance().init(32))
		{
		return __LINE__;
		}

	if(Engine::Render::getInstance().getWindowWidth()<1024 || Engine::Render::getInstance().getWindowHeight()<768)
		{
		LOG_ERROR("Utworzone okno jest za male, AAAAAAAAAAAaaaaaaaaaa");
		/*if(app.pushState(new Game::State::Error()))
			{
			app.run();
			}*/
		}
	else
		{
		//if(app.pushState(new Game::State::Debug()))
		//if(app.pushState(new Game::State::TowerDefense()))
		if(app.pushState(new Game::State::Menu()))
			{
			app.run();
			}
		}

	app.clear();
	Engine::Sound::getInstance().clear();
	Engine::Render::getInstance().clear();

	LOG_INFO("Stop");

	return 0;
	}


