/*
 * musicbox.cpp
 *
 *  Created on: 16 sie 2018
 *      Author: crm
 */

#include "musicbox.h"

using namespace Game;

void MusicBox::update(float dt)
	{
	timer+=dt*speed;

	if(timer<1.0f)
		return;

	timer-=1.0f;

	// Zagraj nutkę
	const char NOTE=notes[step];

	if(NOTE>='A' && NOTE<='H' && NOTE!='B')
		{
		//
		}

	// Inkrementacja kroku
	step++;
	if(step>=notes.size())
		{
		step=0u;
		}
	}
