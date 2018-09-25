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
	const std::string TUTORIAL_01="Sterowanie:\n - Przytrzymaj prawy przycisk myszy aby przesuwa� kamer� (lub naci�nij strza�ki na klawiaturze).\n - Rolka: obr�t kamery\n - CTRL+rolka: przybli�enie/oddalenie kamery.\n\nPlansza sk�ada si� z p�l neutralnych, wrogich (ja�niejszych), oraz twoich (ciemniejszych). Wie�e mo�esz budowa� na swoich polach. Kliknij przycisk z lewej i wstaw wie��.\nNaci�nij przycisk z ��tym wielokropkiem po lewej aby przej�� dalej.";	
	const std::string TUTORIAL_02="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_BUILD_COST+"\".\nWstawienie wie�y powoduje zu�ycie zasob�w (lewy g�rny r�g).";
	const std::string TUTORIAL_03="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_ENEMY_SPAWN+"\".\nZ nory falami wychodz� wrogowie. Zabita jednostka zostawia dusz�, po kt�r� z twojej bazy wychodz� akolici. Odes�ana dusza staje si� twoimi zasobami.";
	const std::string TUTORIAL_04="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_DRAIN_HP+"\".\nKa�dy wr�g kt�ry dojdzie do twojej bazy zmniejszy stan twojego zdrowia o 1 (HP, lewy g�rny r�g).";
	const std::string TUTORIAL_05="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_DRAIN_RESOURCES+"\".\nWie�e i zaj�te przez ciebie pola powoli zu�ywaj� zasoby. Gdy zasoby spadn� do zera, zaczynasz powoli traci� zdrowie. Tempo zu�ycia zasob�w pokazane jest pod zasobami (lewy g�rny r�g).";
	const std::string TUTORIAL_06="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_ENEMY_SPREAD+"\".\nCo pewien czas kr�licze nory b�d� wchodzi�y w stan rozbudowy. Gdy rozbudowa si� zako�czy, na losowym polu obok pojawi si� kolejna nora.";
	const std::string TUTORIAL_07="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_PREFER_CARROTS+"\".\nWrogowie przechodz�c ko�o pola marchewkowego na kt�rym s� marchewki b�d� zbaczali z trasy.";
	const std::string TUTORIAL_08="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_AVOID_TURRETS+"\".\nWrogowie b�d� starali si� unika� wchodzenia na miny lub przechodzenia ko�o twoich wie�.";
	const std::string TUTORIAL_09="W��czona zosta�a regu�a: \""+MENU_CHK_RULE_PLAYER_STOMP+"\".\nAkolici chodz�c po polach marchewkowych b�d� powodowali znikanie marchewek (wrogie jednostki robi� to zawsze).";
	const std::string TUTORIAL_10="Aby wygra�: zbuduj pola marchewkowe na wszystkich polach. Aby pozby� si� kr�liczej nory, postaw ko�o niej wie�� atakuj�c� obszarowo (W, czerwona dziura).\n\n...i to by by�o na tyle w kwestii podstaw gry. Wci�nij ESC i wr�� do menu g��wnego aby rozpocz�� zwyk�� gr�. Menu pomoc zawiera szczeg�owe informacje o mechanice gry.";
	}


