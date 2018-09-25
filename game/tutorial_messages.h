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
#include "gui_messages.h"

namespace Game
	{
	//RULE_NO_RULES
	//RULE_BUILDING_COST
	//RULE_ENEMY_SPAWN_UNITS
	//RULE_DRAIN_HP
	//RULE_DRAIN_RESOURCES
	//RULE_ENEMY_BUILD_TURRETS
	//RULE_ENEMY_PREFER_CARROTS
	//RULE_ENEMY_AVOID_TURRETS
	//RULE_PLAYER_STOMPS_CARROTS
	const std::string TUTORIAL_01="Sterowanie:\n - Przytrzymaj prawy przycisk myszy aby przesuwaæ kamerê (lub naciœnij strza³ki na klawiaturze).\n - Rolka: obrót kamery\n - CTRL+rolka: przybli¿enie/oddalenie kamery.\n\nPlansza sk³ada siê z pól neutralnych, wrogich (jaœniejszych), oraz twoich (ciemniejszych). Wie¿e mo¿esz budowaæ na swoich polach. Kliknij przycisk z lewej i wstaw wie¿ê.\nNaciœnij przycisk z ¿ó³tym wielokropkiem po lewej aby przejœæ dalej.";	
	const std::string TUTORIAL_02="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_BUILD_COST+"\".\nWstawienie wie¿y powoduje zu¿ycie zasobów (lewy górny róg).";
	const std::string TUTORIAL_03="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_ENEMY_SPAWN+"\".\nZ nory falami wychodz¹ wrogowie. Zabita jednostka zostawia duszê, po któr¹ z twojej bazy wychodz¹ akolici. Odes³ana dusza staje siê twoimi zasobami.";
	const std::string TUTORIAL_04="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_DRAIN_HP+"\".\nKa¿dy wróg który dojdzie do twojej bazy zmniejszy stan twojego zdrowia o 1 (HP, lewy górny róg).";
	const std::string TUTORIAL_05="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_DRAIN_RESOURCES+"\".\nWie¿e i zajête przez ciebie pola powoli zu¿ywaj¹ zasoby. Gdy zasoby spadn¹ do zera, zaczynasz powoli traciæ zdrowie. Tempo zu¿ycia zasobów pokazane jest pod zasobami (lewy górny róg).";
	const std::string TUTORIAL_06="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_ENEMY_SPREAD+"\".\nCo pewien czas królicze nory bêd¹ wchodzi³y w stan rozbudowy. Gdy rozbudowa siê zakoñczy, na losowym polu obok pojawi siê kolejna nora.";
	const std::string TUTORIAL_07="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_PREFER_CARROTS+"\".\nWrogowie przechodz¹c ko³o pola marchewkowego na którym s¹ marchewki bêd¹ zbaczali z trasy.";
	const std::string TUTORIAL_08="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_AVOID_TURRETS+"\".\nWrogowie bêd¹ starali siê unikaæ wchodzenia na miny lub przechodzenia ko³o twoich wie¿.";
	const std::string TUTORIAL_09="W³¹czona zosta³a regu³a: \""+MENU_CHK_RULE_PLAYER_STOMP+"\".\nAkolici chodz¹c po polach marchewkowych bêd¹ powodowali znikanie marchewek (wrogie jednostki robi¹ to zawsze).";
	const std::string TUTORIAL_10="Aby wygraæ: zbuduj pola marchewkowe na wszystkich polach. Aby pozbyæ siê króliczej nory, postaw ko³o niej wie¿ê atakuj¹c¹ obszarowo (W, czerwona dziura).\n\n...i to by by³o na tyle w kwestii podstaw gry. Wciœnij ESC i wróæ do menu g³ównego aby rozpocz¹æ zwyk³¹ grê. Menu pomoc zawiera szczegó³owe informacje o mechanice gry.";
	}


