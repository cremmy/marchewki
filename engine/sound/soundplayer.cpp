/*
 * soundplayer.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "soundplayer.h"

#include "../debug/log.h"
#include "../io/resource.h"

using namespace Engine::Sound;

SoundPlayer* SoundPlayer::instance; // Hmm... Czy to nie powinno byc wyzerowane? =D

SoundPlayer::SoundPlayer(): currentMusic(nullptr), nextFadeIn(0u), nextMusic("")
	{
	//
	}

SoundPlayer::~SoundPlayer()
	{
	for(auto it: chunksNamed)
		Mix_FreeChunk(it.second);
	chunksNamed.clear();

	if(currentMusic)
		Mix_FreeMusic(currentMusic);
	currentMusic=nullptr;

	nextMusic="";
	}

bool SoundPlayer::init(unsigned channels, unsigned sampling)
	{
	LOG_INFO("SoundPlayer.init: Startowanie dzwieku...");
	LOG_DEBUG("SoundPlayer.init: [channels: %d][sampling: %d]", channels, sampling);

	unsigned mask=MIX_INIT_OGG;

	if((Mix_Init(mask)&mask)!=mask)
		{
		LOG_ERROR("SoundPlayer.init: Nie udalo sie uruchomic SDL_Mixera");
		LOG_ERROR("SoundPlayer.init: %s", Mix_GetError());
		return false;
		}

	LOG_DEBUG("SoundPlayer.init: Sterowniki:");
	for(int i=0; i<SDL_GetNumAudioDrivers(); ++i)
		LOG_DEBUG("SoundPlayer.init:     [%2d]: %s", i, SDL_GetAudioDriver(i));

	LOG_DEBUG("SoundPlayer.init: Urzadzenia:");
	for(int i=0; i<SDL_GetNumAudioDevices(0); ++i)
		LOG_DEBUG("SoundPlayer.init:     [%2d]: %s", i, SDL_GetAudioDeviceName(i, 0));

	if(Mix_OpenAudio(sampling, MIX_DEFAULT_FORMAT, 2, 2048)!=0)
		{
		LOG_ERROR("SoundPlayer.init: Nie udalo sie uruchomic dzwieku");
		LOG_ERROR("SoundPlayer.init: %s", Mix_GetError());
		return false;
		}

	LOG_DEBUG("SoundPlayer.init: Wybrany sterownik: %s", SDL_GetCurrentAudioDriver());

	int gotchannels=Mix_AllocateChannels(channels);

	if(gotchannels!=(int)channels)
		{
		if(gotchannels<1)
			{
			LOG_ERROR("SoundPlayer.init: Nie udalo sie zaalokowac zadanej liczby kanalow (%d)", channels);
			LOG_ERROR("SoundPlayer.init: %s", Mix_GetError());
			return false;
			}

		LOG_WARNING("SoundPlayer.init: Zadano %u kanalow, otrzymano %d", channels, gotchannels);
		}

	LOG_SUCCESS("SoundPlayer.init: Uruchomiono dzwiek");

	return true;
	}

void SoundPlayer::update()
	{
	if(nextMusic.empty())
		return;

	if(Mix_PlayingMusic())
		return;

	music(nextMusic, nextFadeIn, 0u);
	}

void SoundPlayer::clear()
	{
	Mix_CloseAudio();
	Mix_Quit();
	}

void SoundPlayer::play(const std::string& path, int volume, int channel, int loops)
	{
	Mix_Chunk* chunk=loadSample(path);

	if(!chunk)
		return;

	const int CHANNEL_RECVD=Mix_PlayChannel(-1, chunk, loops);
	if(CHANNEL_RECVD<0)
		{
		LOG_ERROR("SoundPlayer.play: Nie udalo sie uruchomic dzwieku \"%s\"", path.c_str());
		return;
		}

	LOG_DEBUG("SoundPlayer.play: Odtwarzanie \"%s\", kanal %d, glosnosc %d, petle %d", path.c_str(), volume, channel, loops);

	Mix_Volume(CHANNEL_RECVD, volume);
	}


void SoundPlayer::music(const std::string& path, unsigned fadeinms, unsigned fadeoutms)
	{
	LOG_INFO("SoundPlayer.music: Odtwarzanie muzyki \"%s\"", path.c_str());

	if(Mix_PlayingMusic())
		{
		if(fadeoutms>0u)
			{
			LOG_DEBUG("SoundPlayer.music: Muzyka gra (fadeout %ums)", fadeoutms);

			nextFadeIn=fadeinms;
			nextMusic=path;

			Mix_FadeOutMusic(fadeoutms);
			return;
			}

		LOG_DEBUG("SoundPlayer.music: Muzyka gra, zatrzymywanie");

		Mix_HaltMusic();
		Mix_FreeMusic(currentMusic);
		}

	//currentMusic=Mix_LoadMUS(path.c_str());
	currentMusic=Mix_LoadMUS_RW(Engine::IO::Resource::getRW(path), 1);

	if(!currentMusic)
		{
		LOG_ERROR("SoundPlayer.music: Nie udalo sie wczytac pliku muzyki \"%s\"", path.c_str());
		return;
		}

	LOG_DEBUG("SoundPlayer.music: Muzyka wczytana, startowanie (fadein: %ums)", fadeinms);

	if(fadeinms)
		Mix_FadeInMusic(currentMusic, -1, fadeinms);
	else
		Mix_PlayMusic(currentMusic, -1);
	}

void SoundPlayer::stop(unsigned fadeoutms)
	{
	if(fadeoutms>0u)
		Mix_FadeOutMusic(fadeoutms);
	else
		Mix_HaltMusic();
	}

Mix_Chunk* SoundPlayer::loadSample(const std::string& path)
	{
	auto it=chunksNamed.find(path);

	if(it!=chunksNamed.end())
		return it->second;

	LOG_INFO("SoundPlayer.loadSample: Wczytywanie dzwieku \"%s\"", path.c_str());

	Mix_Chunk* chunk=Mix_LoadWAV(path.c_str());
	//Mix_LoadMUS_RW();

	if(!chunk)
		{
		LOG_ERROR("SoundPlayer.loadSample: Nie udalo sie wczytac dzwieku \"%s\"", path.c_str());
		LOG_ERROR("SoundPlayer.loadSample: %s", Mix_GetError());
		return nullptr;
		}

	chunksNamed[path]=chunk;

	return chunk;
	}
