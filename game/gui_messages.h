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
	
	
	const std::string MENU_BTN_NEW_GAME=            "Nowa gra";
	const std::string MENU_BTN_TUTORIAL=            "Tutorial";
	const std::string MENU_BTN_OPTIONS=             "Opcje";
	const std::string MENU_BTN_HELP=                "Pomoc";
	const std::string MENU_BTN_EXIT=                "Wyj�cie";
	const std::string MENU_BTN_GAME_SANDBOX=        "Sandbox";
	const std::string MENU_BTN_GAME_EASY=           "�atwy";
	const std::string MENU_BTN_GAME_MEDIUM=         "�redni";
	const std::string MENU_BTN_GAME_HARD=           "Trudny";
	const std::string MENU_BTN_GAME_CUSTOM=         "W�asne zasady";
	const std::string MENU_BTN_BACK=                "Powr�t";
	const std::string MENU_BTN_START=               "Start";
	const std::string MENU_CHK_RULE_BUILD_COST=     "Gracz: Koszty budowania";
	const std::string MENU_CHK_RULE_ENEMY_SPAWN=    "Wr�g: Tworzenie jednostek";
	const std::string MENU_CHK_RULE_DRAIN_HP=       "Gracz: Utrata zdrowia";
	const std::string MENU_CHK_RULE_DRAIN_RESOURCES="Gracz: Zu�ywanie zasob�w";
	const std::string MENU_CHK_RULE_ENEMY_SPREAD=   "Wr�g: Rozbudowa";
	const std::string MENU_CHK_RULE_PREFER_CARROTS= "Wr�g: Preferowanie marchewek";
	const std::string MENU_CHK_RULE_AVOID_TURRETS=  "Wr�g: Unikanie wie�";
	const std::string MENU_CHK_RULE_PLAYER_STOMP=   "Gracz: Deptanie marchewek";
	const std::string MENU_CHK_SOUNDS=              "D�wi�ki";
	const std::string MENU_CHK_MUSIC=               "Muzyka";
	const std::string MENU_VAL_LEVEL_WIDTH=         "Szeroko�� planszy";
	const std::string MENU_VAL_LEVEL_HEIGHT=        "Wysoko�� planszy";
	const std::string MENU_BTN_RESUME_GAME=         "Powr�t do gry";
	const std::string MENU_BTN_RETURN_TO_MENU=      "Powr�t do menu";
	const std::string MENU_BTN_RETURN_TO_OS=        "Zamknij program";
	}


