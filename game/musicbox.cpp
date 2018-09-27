/*
 * musicbox.cpp
 *
 *  Created on: 16 sie 2018
 *      Author: crm
 */

#include "musicbox.h"

#include "../engine/debug/log.h"
#include "../engine/sound/soundplayer.h"

using namespace Game;

MusicBox::Note getNote(char cnote)
	{
	switch(cnote)
		{
		case 'C': return MusicBox::NOTE_C;
		case 'D': return MusicBox::NOTE_D;
		case 'E': return MusicBox::NOTE_E;
		case 'F': return MusicBox::NOTE_F;
		case 'G': return MusicBox::NOTE_G;
		case 'A': return MusicBox::NOTE_A;
		case 'H': return MusicBox::NOTE_H;

		default:  return MusicBox::NOTE_INVALID;
		}
	}

const char* getNotePath(MusicBox::Note note)
	{
	switch(note)
		{
		case MusicBox::NOTE_C: return "sounds/note_1c.ogg";
		case MusicBox::NOTE_D: return "sounds/note_1d.ogg";
		case MusicBox::NOTE_E: return "sounds/note_1e.ogg";
		case MusicBox::NOTE_F: return "sounds/note_1f.ogg";
		case MusicBox::NOTE_G: return "sounds/note_1g.ogg";
		case MusicBox::NOTE_A: return "sounds/note_2a.ogg";
		case MusicBox::NOTE_H: return "sounds/note_2b.ogg";

		case MusicBox::NOTE_C_CREEPY: return "sounds/note_1c_c.ogg";
		case MusicBox::NOTE_D_CREEPY: return "sounds/note_1d_c.ogg";
		case MusicBox::NOTE_E_CREEPY: return "sounds/note_1e_c.ogg";
		case MusicBox::NOTE_F_CREEPY: return "sounds/note_1f_c.ogg";
		case MusicBox::NOTE_G_CREEPY: return "sounds/note_1g_c.ogg";
		case MusicBox::NOTE_A_CREEPY: return "sounds/note_2a_c.ogg";
		case MusicBox::NOTE_H_CREEPY: return "sounds/note_2b_c.ogg";

		default:               return "sounds/note_1c.ogg";
		}
	}

void MusicBox::update(float dt)
	{
	const float SPEED=0.15f;

	timer+=dt*speed;

	if(timer<SPEED)
		return;

	timer-=SPEED;

	if(wait>0)
		{
		--wait;
		return;
		}

	// Zagraj nutkę
	const char CNOTE=notes[step];
	const Note  NOTE=getNote(CNOTE);

	// Inkrementacja kroku
	++step;
	if(step>=notes.size())
		{
		step=0u;
		}

	if(CNOTE>'0' && CNOTE<='9')
		{
		wait=CNOTE-'0';
		LOG_DEBUG("wait: %d", wait);
		return;
		}

	if(NOTE!=NOTE_INVALID)
		{
		int offset=(rand()/(RAND_MAX+1.0f)<=creepiness)?(NOTE_C_CREEPY-NOTE_C):0;
		LOG_DEBUG("off: %d", offset);

		Engine::Sound::getInstance().play(getNotePath((Note)((int)NOTE+offset)), 32);
		}
	}
