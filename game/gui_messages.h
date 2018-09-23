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
	
	
	const std::string MENU_BTN_NEW_GAME=            "Nowa gra";
	const std::string MENU_BTN_TUTORIAL=            "Tutorial";
	const std::string MENU_BTN_OPTIONS=             "Opcje";
	const std::string MENU_BTN_HELP=                "Pomoc";
	const std::string MENU_BTN_EXIT=                "Wyjœcie";
	const std::string MENU_BTN_GAME_SANDBOX=        "Sandbox";
	const std::string MENU_BTN_GAME_EASY=           "£atwy";
	const std::string MENU_BTN_GAME_MEDIUM=         "Œredni";
	const std::string MENU_BTN_GAME_HARD=           "Trudny";
	const std::string MENU_BTN_GAME_CUSTOM=         "W³asne zasady";
	const std::string MENU_BTN_BACK=                "Powrót";
	const std::string MENU_BTN_START=               "Start";
	const std::string MENU_CHK_RULE_BUILD_COST=     "Gracz: Koszty budowania";
	const std::string MENU_CHK_RULE_ENEMY_SPAWN=    "Wróg: Tworzenie jednostek";
	const std::string MENU_CHK_RULE_DRAIN_HP=       "Gracz: Utrata zdrowia";
	const std::string MENU_CHK_RULE_DRAIN_RESOURCES="Gracz: Zu¿ywanie zasobów";
	const std::string MENU_CHK_RULE_ENEMY_SPREAD=   "Wróg: Rozbudowa";
	const std::string MENU_CHK_RULE_PREFER_CARROTS= "Wróg: Preferowanie marchewek";
	const std::string MENU_CHK_RULE_AVOID_TURRETS=  "Wróg: Unikanie wie¿";
	const std::string MENU_CHK_RULE_PLAYER_STOMP=   "Gracz: Deptanie marchewek";
	const std::string MENU_CHK_SOUNDS=              "DŸwiêki";
	const std::string MENU_CHK_MUSIC=               "Muzyka";
	const std::string MENU_VAL_LEVEL_WIDTH=         "Szerokoœæ planszy";
	const std::string MENU_VAL_LEVEL_HEIGHT=        "Wysokoœæ planszy";
	const std::string MENU_BTN_RESUME_GAME=         "Powrót do gry";
	const std::string MENU_BTN_RETURN_TO_MENU=      "Powrót do menu";
	const std::string MENU_BTN_RETURN_TO_OS=        "Zamknij program";
	}


