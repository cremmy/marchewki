/*
 * soundplayer.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <vector>
#include <map>
#include <string>
#include <SDL2/SDL_mixer.h>

namespace Engine
	{
	namespace Sound
		{
		class SoundPlayer
			{
			/** TODO
			 * - Ustawianie globalnej glosnosci
			 * - Zatrzymanie wszystkich dzwiekow
			 * - Lepsze zarzadzanie pamiecia
			 * - Osobny watek uruchamiany przy przelaczaniu muzyki (wywolujacy update)/crossfade?
			 */

			protected:
				static SoundPlayer* instance;

				SoundPlayer();
				~SoundPlayer();

				Mix_Chunk* loadSample(const std::string& path);
				//SDL_Thread thread;

				std::map<std::string, Mix_Chunk*> chunksNamed;

				Mix_Music *currentMusic;
				unsigned nextFadeIn;
				std::string nextMusic;

			public:
				inline static SoundPlayer& getInstance()
					{
					if(!instance)
						instance=new SoundPlayer();

					return *instance;
					}

				bool init(unsigned channels, unsigned sampling=44100);
				void update();
				void clear();

				void play(const std::string& path, int volume=128, int loops=0u) {play(path, volume, -1, loops);}
				void play(const std::string& path, int volume, int channel, int loops);

				void music(const std::string& path, unsigned fadeinms=0u, unsigned fadeoutms=500u);
				void stop(unsigned fadeoutms=500u);
			};

		inline SoundPlayer& getInstance() {return SoundPlayer::getInstance();}
		} /* namespace Sound */
	} /* namespace Engine */
