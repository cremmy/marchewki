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
	//const std::string MENU_BTN_BACK=            "Powr�t";
	const std::string MENU_BTN_BASICS=          "Podstawy";
	const std::string MENU_BTN_TURRETS=         "Budynki";
	const std::string MENU_BTN_TURRET_SINGLE=   "Czaszka kr�lika na kiju";
	const std::string MENU_BTN_TURRET_AOE=      "Czerwona dziura";
	const std::string MENU_BTN_TURRET_MINE=     "Pentagram";
	const std::string MENU_BTN_TURRET_CARROTS=  "Marchewkowe pole";
	const std::string MENU_BTN_TURRET_BASE=     "Baza gracza";
	const std::string MENU_BTN_TURRET_SPAWNER=  "Kr�licza nora";
	const std::string MENU_BTN_ENEMIES=         "Wrogowie";
	//const std::string MENU_BTN_RULES=           "Regu�y";
	
	const std::string HELP_BASICS=
		"Aby wygra�:\n 1. Zniszcz wszystkie kr�licze nory\n 2. Zbuduj marchewkowe pola na ka�dym polu.\n\n"
		"Sterowanie:\n Lewy przycisk myszy: Wyb�r pola (do ulepszenia lub sprzedania), zatwierdzenie budowy wie�y\n Prawy przycisk myszy: ruch kamery\n Rolka: obr�t kamery\n CTRL+Rolka: przybli�enie/oddalenie kamery\n Q/W/E/R: Budowanie wie�y\n A: Ulepszenie zaznaczonej wie�y\n C: Sprzeda� zaznaczonej wiezy\n Spacja: Przy�pieszenie gry\n ESC: Menu\n\n"
		"Plansza sk�ada si� z trzech rodzaj�w p�l: nale��cych do gracza, wroga, lub neutralnych. Wie�e mo�esz zbudowa� tylko na polu nale��cym do ciebie. "
		"Ka�da wie�a ma swoj� stref� wp�ywu i przejmuje pewn� liczb� s�siednich p�l (zale�nie od poziomu ulepszenia).\n"
		"Zbudowanie wie�y kosztuje okre�lon� ilo�� zasob�w. Dodatkowo, zasoby zu�ywane s� stale przez wie�e oraz zaj�te pola. Zabite jednostki wroga pozostawiaj� po sobie dusz�, po kt�r� z bazy gracza co pewien czas wychodz� akolici. "
		"Odes�anie duszy daje zasoby, zale�ne od poczatkowego poziomu zdrowia jednostki. Akolici zbieraj� dusze w kolejno�ci ich pojawienia si� na mapie.";
		
	const std::string HELP_TURRET_SINGLE=
		"Czaszka kr�lika na kiju, wie�a atakuj�ca pojedynczy cel, Q\n\n"
		"Ta wie�a atakuje pojedyncze cele. Co pewien czas (zale�ny od poziomu ulepszenia) wystrzeliwuje pocisk, kt�ry leci prosto do wybranego celu i przy trafieniu os�abia tylko jego (obra�enia zale�ne od poziomu ulepszenia).\n"
		"Wie�a szuka najbli�szego celu i atakuje go dop�ki nie zginie, lub nie oddali si� poza zasi�g strza�u.\nZadaje zwi�kszone obra�enia opancerzonym jednostkom.";
	const std::string HELP_TURRET_AOE=
		"Czerwona dziura, wie�a obszarowa, W\n\n"
		"Ta wie�a wystrzeliwuje pociski, kt�re po zderzeniu z ziemi� wybuchaj�, os�abiaj�c wszystkich w pewnym zasi�gu. Pociski wybuchaj�c na polu z kr�licza nor� powoli j� os�abiaj�.\n"
		"Postawienie wiezy bezpo�rednio przy kr�liczej norze powoduje jej zniszczenie.\n"
		"Wie�a szuka najbli�szego celu i atakuje go dop�ki nie zginie, lub nie oddali si� poza zasi�g strza�u.\n Zadaje zmniejszone obra�enia opancerzonym jednostkom.";
	const std::string HELP_TURRET_MINE=
		"Pentagram, mina, E\n\n"
		"Ta 'wie�a' nie blokuje przej�cia. Gdy na polu z ni� stanie co najmniej [POZIOM ULEPSZENIA]+1 wrog�w, wszystkie wrogie jednostki zostan� os�abione, a mina chwilowo zostanie zdezaktywowana. Zaraz po wybudowaniu, mina wybucha gdy stanie na niej pierwsza jednostka wroga.\n"
		"Gdy mina jest nieaktywna po wybuchu, to powoli generuje zasoby (ilo�� zale�na od poziomu ulepszenia).\n"
		"Usuni�cie miny zu�ywa zasoby.\nZadaje zwi�kszone obra�enia lekkim jednostkom, i zmniejszone opancerzonym.";
	const std::string HELP_TURRET_CARROTS=
		"Marchewkowe pole, R\n\n"
		"Wie�a pozbawiona w�asno�ci ofensywnych. Co pewien czas na polu wyrastaj� kolejne marchewki. Lekkie jednostki wroga kr��� po polu dop�ki wszystkie marchewki nie zostan� zjedzone.\n"
		"Zabudowanie wszystkich p�l w grze polami marchewkowymi jest warunkiem wygrania gry.\n"
		"Pole marchewkowe mo�e zosta� zniszczone przez rozbudowuj�c� si� w pobli�u kr�licz� nor�.";
	const std::string HELP_TURRET_BASE=
		"Baza gracza, wiejska chatka\n\n"
		"Budynek do kt�rego zmierzaj� jednostki wroga. Je�li na planszy s� niezebrane dusze, co pewien czas wypuszcza akolit�w, kt�rzy id� je zebra�.\n"
		"Poziom ulepszenia zmienia si� zale�nie od procentowego zaj�cia przez gracza p�l. Zu�ycie zasob�w jest zale�ne od poziomu ulepszenia.";
	const std::string HELP_TURRET_SPAWNER=
		"Kr�licza nora, dziura, spawner\n\n"
		"Dziura z kt�rej falami wychodz� dwa rodzaje wrogich jednostek: lekkie, oraz opancerzone. Co pewien czas nora wchodzi w stan rozbudowy (��te ta�my roz�o�one wok�). Po chwili, w niewielkiej odleg�o�ci mo�e pojawi� si� kolejna nora.\n"
		"Je�li nie uda si� znale�� miejsca na kolejn� dziur�, lub gdy liczba dziur w s�siedztwie jest wi�ksza od pewnej warto�ci, nora mo�e zdoby� b�ogos�awie�stwo. "
		"Gdy w bezpo�rednim s�siedztwie znajduje si� przynajmniej jedna czerwona dziura (wie�a obszarowa), lub gdy poziom zdrowia nory spadnie poni�ej pewnego poziomu, nora wchodzi w stan paniki.\n\n"
		"B�ogos�awie�stwo:\n Z nory wyrusza krucjata - kilka pot�nych fal jednostek opancerzonych.\n\n"
		"Panika:\n Z nory wychodzi szybsza i silniejsza fala jednostek. Je�li podczas stanu paniki poziom zdrowia spadnie do zera, lub czerwona dziura (wie�a obszarowa) dalej b�dzie w s�siedztwie, to nora zostanie zniszczona.";
	const std::string HELP_ENEMIES=
		"W grze pojawiaj� si� trzy rodzaje jednostek: akolici (gracz), kr�liki (wr�g), oraz kr�liki-krzy�owcy (wr�g).\n\n"
		"Akolita\n Jednostka nale��ca do gracza. Odsy�a dusze znajduj�ce si� na polu gry, co dostarcza graczowi zasob�w. Akolici co pewien czas wychodz� z bazy i zbieraj� dusze w takiej kolejno�ci, w jakiej pojawi�y si� one na mapie.\n\n"
		"Kr�lik\n Lekka jednostka, otrzymuje zwi�kszone obra�enia od min (pentagram). Je�li na polu marchewkowym po kt�rym idzie s� marchewki, to kr��y po nim dop�ki nie zje wszystkich.\n\n"
		"Kr�lik-krzy�owiec\n Jednostka opancerzona, otrzymuje zwi�kszone obra�enia od atak�w os�abiaj�cych pojedynczy cel i zmniejszone obra�enia obszarowe. Nie zjada marchewek (nie mieszcz� si� w he�mie). Po przej�ciu kilku p�l staje si� zm�czony i zatrzymuje si� na chwil� co ka�de pole.\n\n";
	const std::string HELP_RULES=
		"W grze mo�na wybra� dowolny zestaw regu�.\n "+
		MENU_CHK_RULE_BUILD_COST+" - Je�li ta regu�a jest aktywna, to budowanie wie� wymaga zasob�w i je zu�ywa.\n "+
		MENU_CHK_RULE_ENEMY_SPAWN+" - Je�li ta regu�a jest aktywna, to z kr�liczych nor wychodz� wrogowie.\n "+
		MENU_CHK_RULE_DRAIN_HP+" - Jesli ta regu�a jest aktywna, to jednostki wroga zmniejsz� stan zdrowia gracza gdy uda im si� doj�� do bazy.\n "+
		MENU_CHK_RULE_DRAIN_RESOURCES+" - Je�li ta regu�a jest aktywna, to wie�e i pola zaj�te przez gracza zu�ywaj� zasoby.\n "+
		MENU_CHK_RULE_ENEMY_SPREAD+" - Je�li ta regu�a jest aktywna, to kr�licze nory b�d� co jaki� czas rozprzestrzenia�y si� na s�siednie pola.\n "+
		MENU_CHK_RULE_PREFER_CARROTS+" - Je�li ta regu�a jest aktywna, to lekkie jednostki wroga b�d� wola�y wej�� na pole marchewkowe z marchewkami, zamiast i�� do celu.\n "+
		MENU_CHK_RULE_AVOID_TURRETS+" - Je�li ta regu�a jest aktywna, to jednostki wroga b�d� stara�y si� unika� wie� gracza. Oznacza to, �e b�d� wybiera�y �cie�k� ko�o kt�rej jest jak najmniej wie� gracza (zwracaj�c szczeg�ln� uwag� na miny).\n "+
		MENU_CHK_RULE_PLAYER_STOMP+" - Je�li ta regu�a jest aktywna, to jednostki gracza chodz�ce po polach marchewkowych b�d� niszczy�y marchewki.";
	}


