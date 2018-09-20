/*
 * gui_messages.h
 *
 *  Created on: 18 wrz 2018
 *      Author: crm
 */

//
// !! Pamiêtaj o zmianie kodowania na ANSI !!
//    ^ Polskie znaki w Engine::UI::Text
 
#pragma once

#include <string>

namespace Game
	{
	const std::string GUI_MESSAGE_BTN_TURRET_SINGLE="Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuniêcie: %s\nAtakuje pojedynczy cel. Zwiêkszone obra¿enia przeciwko opancerzonym jednostkom. [Q]";
	const std::string GUI_MESSAGE_BTN_TURRET_AOE=   "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuniêcie: %s\nPocisk wybucha, os³abiaj¹c wszystkich w zasiêgu. Zmniejszone obra¿enia przeciwko opancerzonym jednostkom. Niszczy wie¿e wroga. [W]";
	const std::string GUI_MESSAGE_BTN_TURRET_MINE=  "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuniêcie: %s\nWybucha gdy na polu jest [POZIOM]+1 jednostek. Generuje zasoby gdy nieaktywne. Zwiêkszone obra¿enia przeciwko lekkim jednostkom. [E]";
	const std::string GUI_MESSAGE_BTN_CARROT_FIELD= "Koszt: %c (Utrzymanie: %d)\nUlepszenie: %u\nUsuniêcie: %s\nPole marchewkowe. Nie zu¿ywa zasobów. Aby wygraæ, na ca³ej planszy posadŸ marchewki. [R]";
	const std::string GUI_MESSAGE_BTN_UPGRADE=      "Koszt: %c\nUlepszenie zaznaczonego obiektu. [A]";
	const std::string GUI_MESSAGE_BTN_SELL=         "Koszt: %c\nUsuniêcie zaznaczonego obiektu. [C]";
	
	
	const std::string MENU_BTN_NEW_GAME=         "Nowa gra";
	const std::string MENU_BTN_TUTORIAL=         "Samouczek";
	const std::string MENU_BTN_OPTIONS=          "Opcje";
	const std::string MENU_BTN_EXIT=             "Wyjœcie";
	}


