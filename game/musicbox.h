/*
 * musicbox.h
 *
 *  Created on: 16 sie 2018
 *      Author: crm
 */

#pragma once

#include <string>

const char VIVALDI[]="C1E1E1E1D C G4"
					 "G F E1E1E1D C G4"
					 "G F E1F G F1E1D3"
					 "C1E1E1E1D C G4"
					 "G F E1E1E1D C G4"
					 "G F E1F G F1E2D4"
					 "C2G1F E F G1A1G4"
					 "C2G1F E F G1A1G4"
					 "C A1G3F E D C D C   "
					 "C2G1F E F G1A1G4"
					 "C2G1F E F G1A1G4"
					 "C A G   F E D C D C    ";

namespace Game
	{
	class MusicBox
		{
		public:
			enum Note {
				NOTE_C,
				NOTE_D,
				NOTE_E,
				NOTE_F,
				NOTE_G,
				NOTE_A,
				NOTE_H,
				NOTE_C_CREEPY,
				NOTE_D_CREEPY,
				NOTE_E_CREEPY,
				NOTE_F_CREEPY,
				NOTE_G_CREEPY,
				NOTE_A_CREEPY,
				NOTE_H_CREEPY,

				NOTE_INVALID,
			};

		protected:
			float speed;       // Nuty na sekundę
			float timer;       // Licznik czasu
			unsigned step;     // Licznik aktualnej nuty
			int wait;
			std::string notes;

			float creepiness;

		public:
			MusicBox(): speed(1.0f), timer(0.0f), step(0u), wait(0), notes(), creepiness(0.0f)
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
