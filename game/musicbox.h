/*
 * musicbox.h
 *
 *  Created on: 16 sie 2018
 *      Author: crm
 */

#pragma once

#include <string>

const char VIVALDI[]="C E E E D C G"
					 "G F E E E D C G"
					 "G F E F G F E D"
					 "C E E E D C G"
					 "G F E E E D C G"
					 "G F E F G F E D"
					 "C G F E F G A G"
					 "C G F E F G A G"
					 "C A G F E D C D C C"
					 "C G F E F G A G"
					 "C G F E F G A G"
					 "C A G F E D C D C C";

namespace Game
	{
	class MusicBox
		{
		protected:
			enum {
				NOTE_C,
				NOTE_D,
				NOTE_E,
				NOTE_F,
				NOTE_G,
				NOTE_A,
				NOTE_H,
			};

			float speed;       // Nuty na sekundę
			float timer;       // Licznik czasu
			unsigned step;     // Licznik aktualnej nuty
			std::string notes;

			float creepiness;

		public:
			MusicBox(): speed(1.0f), timer(0.0f), step(0u), notes(), creepiness(1.0f)
				{
				//
				}
			virtual ~MusicBox()
				{
				//
				}

			void update(float dt);

			void setNotes(const std::string& s) {timer=0.0f; notes=s;}
			void setSpeed(float s) {if(s<=0.0f) return; speed=s;}
			void setCreepiness(float s) {if(s<0.0f || s>1.0f) return; creepiness=s;}
		};

	} /* namespace Game */
