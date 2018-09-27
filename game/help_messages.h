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
	//const std::string MENU_BTN_BACK=            "Powrót";
	const std::string MENU_BTN_BASICS=          "Podstawy";
	const std::string MENU_BTN_TURRETS=         "Budynki";
	const std::string MENU_BTN_TURRET_SINGLE=   "Czaszka królika na kiju";
	const std::string MENU_BTN_TURRET_AOE=      "Czerwona dziura";
	const std::string MENU_BTN_TURRET_MINE=     "Pentagram";
	const std::string MENU_BTN_TURRET_CARROTS=  "Marchewkowe pole";
	const std::string MENU_BTN_TURRET_BASE=     "Baza gracza";
	const std::string MENU_BTN_TURRET_SPAWNER=  "Królicza nora";
	const std::string MENU_BTN_ENEMIES=         "Wrogowie";
	//const std::string MENU_BTN_RULES=           "Regu³y";
	
	const std::string HELP_BASICS=
		"Aby wygraæ:\n 1. Zniszcz wszystkie królicze nory\n 2. Zbuduj marchewkowe pola na ka¿dym polu.\n\n"
		"Sterowanie:\n Lewy przycisk myszy: Wybór pola (do ulepszenia lub sprzedania), zatwierdzenie budowy wie¿y\n Prawy przycisk myszy: ruch kamery\n Rolka: obrót kamery\n CTRL+Rolka: przybli¿enie/oddalenie kamery\n Q/W/E/R: Budowanie wie¿y\n A: Ulepszenie zaznaczonej wie¿y\n C: Sprzeda¿ zaznaczonej wiezy\n Spacja: Przyœpieszenie gry\n ESC: Menu\n\n"
		"Plansza sk³ada siê z trzech rodzajów pól: nale¿¹cych do gracza, wroga, lub neutralnych. Wie¿e mo¿esz zbudowaæ tylko na polu nale¿¹cym do ciebie. "
		"Ka¿da wie¿a ma swoj¹ strefê wp³ywu i przejmuje pewn¹ liczbê s¹siednich pól (zale¿nie od poziomu ulepszenia).\n"
		"Zbudowanie wie¿y kosztuje okreœlon¹ iloœæ zasobów. Dodatkowo, zasoby zu¿ywane s¹ stale przez wie¿e oraz zajête pola. Zabite jednostki wroga pozostawiaj¹ po sobie duszê, po któr¹ z bazy gracza co pewien czas wychodz¹ akolici. "
		"Odes³anie duszy daje zasoby, zale¿ne od poczatkowego poziomu zdrowia jednostki. Akolici zbieraj¹ dusze w kolejnoœci ich pojawienia siê na mapie.";
		
	const std::string HELP_TURRET_SINGLE=
		"Czaszka królika na kiju, wie¿a atakuj¹ca pojedynczy cel, Q\n\n"
		"Ta wie¿a atakuje pojedyncze cele. Co pewien czas (zale¿ny od poziomu ulepszenia) wystrzeliwuje pocisk, który leci prosto do wybranego celu i przy trafieniu os³abia tylko jego (obra¿enia zale¿ne od poziomu ulepszenia).\n"
		"Wie¿a szuka najbli¿szego celu i atakuje go dopóki nie zginie, lub nie oddali siê poza zasiêg strza³u.\nZadaje zwiêkszone obra¿enia opancerzonym jednostkom.";
	const std::string HELP_TURRET_AOE=
		"Czerwona dziura, wie¿a obszarowa, W\n\n"
		"Ta wie¿a wystrzeliwuje pociski, które po zderzeniu z ziemi¹ wybuchaj¹, os³abiaj¹c wszystkich w pewnym zasiêgu. Pociski wybuchaj¹c na polu z królicza nor¹ powoli j¹ os³abiaj¹.\n"
		"Postawienie wiezy bezpoœrednio przy króliczej norze powoduje jej zniszczenie.\n"
		"Wie¿a szuka najbli¿szego celu i atakuje go dopóki nie zginie, lub nie oddali siê poza zasiêg strza³u.\n Zadaje zmniejszone obra¿enia opancerzonym jednostkom.";
	const std::string HELP_TURRET_MINE=
		"Pentagram, mina, E\n\n"
		"Ta 'wie¿a' nie blokuje przejœcia. Gdy na polu z ni¹ stanie co najmniej [POZIOM ULEPSZENIA]+1 wrogów, wszystkie wrogie jednostki zostan¹ os³abione, a mina chwilowo zostanie zdezaktywowana. Zaraz po wybudowaniu, mina wybucha gdy stanie na niej pierwsza jednostka wroga.\n"
		"Gdy mina jest nieaktywna po wybuchu, to powoli generuje zasoby (iloœæ zale¿na od poziomu ulepszenia).\n"
		"Usuniêcie miny zu¿ywa zasoby.\nZadaje zwiêkszone obra¿enia lekkim jednostkom, i zmniejszone opancerzonym.";
	const std::string HELP_TURRET_CARROTS=
		"Marchewkowe pole, R\n\n"
		"Wie¿a pozbawiona w³asnoœci ofensywnych. Co pewien czas na polu wyrastaj¹ kolejne marchewki. Lekkie jednostki wroga kr¹¿¹ po polu dopóki wszystkie marchewki nie zostan¹ zjedzone.\n"
		"Zabudowanie wszystkich pól w grze polami marchewkowymi jest warunkiem wygrania gry.\n"
		"Pole marchewkowe mo¿e zostaæ zniszczone przez rozbudowuj¹c¹ siê w pobli¿u królicz¹ norê.";
	const std::string HELP_TURRET_BASE=
		"Baza gracza, wiejska chatka\n\n"
		"Budynek do którego zmierzaj¹ jednostki wroga. Jeœli na planszy s¹ niezebrane dusze, co pewien czas wypuszcza akolitów, którzy id¹ je zebraæ.\n"
		"Poziom ulepszenia zmienia siê zale¿nie od procentowego zajêcia przez gracza pól. Zu¿ycie zasobów jest zale¿ne od poziomu ulepszenia.";
	const std::string HELP_TURRET_SPAWNER=
		"Królicza nora, dziura, spawner\n\n"
		"Dziura z której falami wychodz¹ dwa rodzaje wrogich jednostek: lekkie, oraz opancerzone. Co pewien czas nora wchodzi w stan rozbudowy (¿ó³te taœmy roz³o¿one wokó³). Po chwili, w niewielkiej odleg³oœci mo¿e pojawiæ siê kolejna nora.\n"
		"Jeœli nie uda siê znaleŸæ miejsca na kolejn¹ dziurê, lub gdy liczba dziur w s¹siedztwie jest wiêksza od pewnej wartoœci, nora mo¿e zdobyæ b³ogos³awieñstwo. "
		"Gdy w bezpoœrednim s¹siedztwie znajduje siê przynajmniej jedna czerwona dziura (wie¿a obszarowa), lub gdy poziom zdrowia nory spadnie poni¿ej pewnego poziomu, nora wchodzi w stan paniki.\n\n"
		"B³ogos³awieñstwo:\n Z nory wyrusza krucjata - kilka potê¿nych fal jednostek opancerzonych.\n\n"
		"Panika:\n Z nory wychodzi szybsza i silniejsza fala jednostek. Jeœli podczas stanu paniki poziom zdrowia spadnie do zera, lub czerwona dziura (wie¿a obszarowa) dalej bêdzie w s¹siedztwie, to nora zostanie zniszczona.";
	const std::string HELP_ENEMIES=
		"W grze pojawiaj¹ siê trzy rodzaje jednostek: akolici (gracz), króliki (wróg), oraz króliki-krzy¿owcy (wróg).\n\n"
		"Akolita\n Jednostka nale¿¹ca do gracza. Odsy³a dusze znajduj¹ce siê na polu gry, co dostarcza graczowi zasobów. Akolici co pewien czas wychodz¹ z bazy i zbieraj¹ dusze w takiej kolejnoœci, w jakiej pojawi³y siê one na mapie.\n\n"
		"Królik\n Lekka jednostka, otrzymuje zwiêkszone obra¿enia od min (pentagram). Jeœli na polu marchewkowym po którym idzie s¹ marchewki, to kr¹¿y po nim dopóki nie zje wszystkich.\n\n"
		"Królik-krzy¿owiec\n Jednostka opancerzona, otrzymuje zwiêkszone obra¿enia od ataków os³abiaj¹cych pojedynczy cel i zmniejszone obra¿enia obszarowe. Nie zjada marchewek (nie mieszcz¹ siê w he³mie). Po przejœciu kilku pól staje siê zmêczony i zatrzymuje siê na chwilê co ka¿de pole.\n\n";
	const std::string HELP_RULES=
		"W grze mo¿na wybraæ dowolny zestaw regu³.\n "+
		MENU_CHK_RULE_BUILD_COST+" - Jeœli ta regu³a jest aktywna, to budowanie wie¿ wymaga zasobów i je zu¿ywa.\n "+
		MENU_CHK_RULE_ENEMY_SPAWN+" - Jeœli ta regu³a jest aktywna, to z króliczych nor wychodz¹ wrogowie.\n "+
		MENU_CHK_RULE_DRAIN_HP+" - Jesli ta regu³a jest aktywna, to jednostki wroga zmniejsz¹ stan zdrowia gracza gdy uda im siê dojœæ do bazy.\n "+
		MENU_CHK_RULE_DRAIN_RESOURCES+" - Jeœli ta regu³a jest aktywna, to wie¿e i pola zajête przez gracza zu¿ywaj¹ zasoby.\n "+
		MENU_CHK_RULE_ENEMY_SPREAD+" - Jeœli ta regu³a jest aktywna, to królicze nory bêd¹ co jakiœ czas rozprzestrzenia³y siê na s¹siednie pola.\n "+
		MENU_CHK_RULE_PREFER_CARROTS+" - Jeœli ta regu³a jest aktywna, to lekkie jednostki wroga bêd¹ wola³y wejœæ na pole marchewkowe z marchewkami, zamiast iœæ do celu.\n "+
		MENU_CHK_RULE_AVOID_TURRETS+" - Jeœli ta regu³a jest aktywna, to jednostki wroga bêd¹ stara³y siê unikaæ wie¿ gracza. Oznacza to, ¿e bêd¹ wybiera³y œcie¿kê ko³o której jest jak najmniej wie¿ gracza (zwracaj¹c szczególn¹ uwagê na miny).\n "+
		MENU_CHK_RULE_PLAYER_STOMP+" - Jeœli ta regu³a jest aktywna, to jednostki gracza chodz¹ce po polach marchewkowych bêd¹ niszczy³y marchewki.";
	}


