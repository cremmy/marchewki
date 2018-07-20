/*
 * resource.h
 *
 *  Created on: 23-12-2011
 *      Author: crm
 */

#pragma once

#include <string>

#ifndef RESOURCEFILE_NO_SDL_RWOPS
#include <SDL2/SDL.h>
#endif

/****
 * Zebym sobie nie wypominal, ze nie opisuje jak co dziala:
 * Resource obsluguje dwa formaty sciezki do pliku:
 *  1. sciezka/do/resfile.crs:plik/gdzies/w/nim.png
 *     Szuka resfile pod podana sciezka, jesli nie ma - nic nie zwroci. Jesli jest - szuka podanego pliku.
 *     Kazdy otwarty resfile laduje w resfiles i mresfiles. KAZDY.
 *  2. sciezka/do/pliku.bmp
 *     Przeszukuje zaincludowane wczesniej resfile (include, includeDir, includeList) w poszukiwaniu podanego pliku. Jesli nie znajdzie, szuka na dysku. Jesli nie znajdzie, nie zwraca nic.
 *     Przeszukiwany jest tylko vfs i mvfs - tam laduja includowane resfile.
 * Obslugiwane (no, w momencie pisania jeszcze nie d; ) sa symlinki - w mresfiles dodany jest wpis sciezka/symlinkowa/resfile.crs wskazujacy na DOWOLNY inny resfile.
 * Kazdy resfile moze miec miliard symlinkow, jesli tylko miejsca nie zabraknie. Przydatne moze byc jesli nazwa resfile zmieni sie lokalnie i skrypty w srodku beda wymagaly oryginalnej sciezki.
 * Finalnie, sprowadza sie do tego:
 *  - byl plik data/resfile.crs i plik img/blah.png w nim
 *  - jest plik download/baadf00d.crs i plik img/blah.png w nim
 *  - symlinkowanie data/resfile.crs na download/baadf00d.crs
 *  - odwolania po oryginalnej nazwie dalej dzialaja (data/resfile.crs:img/blah.png)
 * Teraz to zaimplementowac i bedzie cudo.
 */

namespace Engine
	{
	namespace IO
		{
		class Resfile;

		namespace Resource
			{
			void clear();
			bool open(const std::string& path);

			bool include(const std::string& path);		// open + dolaczenie do vfs'a
			bool includeDir(const std::string& path);	// jw, calego folderu resfile'ow
			bool includeList(const std::string& path);	// jw, otwiera z listy w pliku ini

			bool find(const std::string& path);
			char* load(const std::string& path);

#ifndef RESOURCEFILE_NO_SDL_RWOPS
			SDL_RWops* getRW(const std::string& path);
#endif

			void symlinkAdd(const std::string& name, const std::string& target);
			void symlinkDel(const std::string& name);

			unsigned getSize(const std::string& file);
			unsigned getSize();
			};
		}
	}
