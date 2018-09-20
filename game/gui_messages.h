/*
 * gui_messages.h
 *
 *  Created on: 18 wrz 2018
 *      Author: crm
 */

//
// !! Pami�taj o zmianie kodowania na ANSI !!
//    ^ Polskie znaki w Engine::UI::Text
 
#pragma once

#include <string>

namespace Game
	{
	const std::string GUI_MESSAGE_BTN_TURRET_SINGLE="Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuni�cie: %s\nAtakuje pojedynczy cel. Zwi�kszone obra�enia przeciwko opancerzonym jednostkom. [Q]";
	const std::string GUI_MESSAGE_BTN_TURRET_AOE=   "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuni�cie: %s\nPocisk wybucha, os�abiaj�c wszystkich w zasi�gu. Zmniejszone obra�enia przeciwko opancerzonym jednostkom. Niszczy wie�e wroga. [W]";
	const std::string GUI_MESSAGE_BTN_TURRET_MINE=  "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuni�cie: %s\nWybucha gdy na polu jest [POZIOM]+1 jednostek. Generuje zasoby gdy nieaktywne. Zwi�kszone obra�enia przeciwko lekkim jednostkom. [E]";
	const std::string GUI_MESSAGE_BTN_CARROT_FIELD= "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuni�cie: %s\nPole marchewkowe. Nie zu�ywa zasob�w. Aby wygra�, na ca�ej planszy posad� marchewki. [R]";
	const std::string GUI_MESSAGE_BTN_UPGRADE=      "Koszt: %c\nUlepszenie zaznaczonego obiektu. [A]";
	const std::string GUI_MESSAGE_BTN_SELL=         "Koszt: %c\nUsuni�cie zaznaczonego obiektu. [C]";
	
	
	const std::string MENU_BTN_NEW_GAME=         "Nowa gra";
	const std::string MENU_BTN_TUTORIAL=         "Samouczek";
	const std::string MENU_BTN_OPTIONS=          "Opcje";
	const std::string MENU_BTN_EXIT=             "Wyj�cie";
	}


