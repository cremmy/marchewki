/*
 * resourcefile.h
 *
 *  Created on: 7 sty 2017
 *      Author: crm
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

#ifndef RESOURCEFILE_NO_SDL_RWOPS
#include <SDL2/SDL.h>
#endif

namespace Engine
	{
	namespace IO
		{
		class ResourceFile
			{
			private:
				enum Flags
					{
					NONE=0x0,
					COMPRESSED=0x1,
					ENCODED=0x2
					};

				struct Item
					{
					Item(): path(), name(), start(0), size(0), ucsize(0), flags(NONE) {};
					Item(const Item& i): path(i.path), name(i.name), start(i.start), size(i.size), ucsize(i.ucsize), flags(i.flags) {}

					std::string path;							// sciezka/do/pliku/blah.png
					std::string name;							// blah.png
					unsigned start, size;						// poczatek i wielkosc pliku
					unsigned ucsize;							// wielkosc pliku po rozpakowaniu
					char flags;									// flagi
					};

				struct TreeItem
					{
					enum Type
						{
						DIRECTORY,
						FILE,
						};

					TreeItem(): type(DIRECTORY), name(), items(), item(NULL) {}

					Type type;
					std::string name;							// Nazwa aktualnego elementu
					std::vector<TreeItem> items;				// Lista plikow i podfolderow
					Item *item;
					};

				std::fstream in;								// Otwarty resfile
				std::string respath;								// Nazwa aktualnie otwartego pliku
				unsigned size, rsize;							// Wielkosc ostatnio odczytanego pliku, wielkosc resfile

				std::vector<Item *> header;						// Naglowek resfile

				TreeItem root;									// Korzen drzewka

				char* load(Item* entry);
				Item* findItem(const std::string& path);

				void treeAddItem(const std::string& path, Item *item);
				TreeItem* treeFind(const std::string& name, TreeItem* leaf, TreeItem::Type type);
				TreeItem* treeFindFile(const std::string& name, TreeItem* leaf) {return treeFind(name, leaf, TreeItem::FILE);}
				TreeItem* treeFindDirectory(const std::string& name, TreeItem* leaf) {return treeFind(name, leaf, TreeItem::DIRECTORY);}

			public:
				ResourceFile(): in(), /*curr(""),*/ size(0), rsize(0) {clear();}
				~ResourceFile() {clear();}

				bool open(const std::string& res);								// Resfile nie bawi sie w dzielenie na sciezka/do/resfile.crs:img/blah.png
				void clear();													// Otwierasz openem i wyciagasz z danego pliku.

				char* load(const std::string& res, const std::string& path);	// res.crs, img/blah.png
				char* load(const std::string& path);							// img/blah.png
				char* data(const std::string& res);

#ifndef RESOURCEFILE_NO_SDL_RWOPS
				SDL_RWops* getRW(const std::string& path);
#endif

				//const std::string& getCurrent() const {return curr;}
				bool find(const std::string& path);

				unsigned getSize(const std::string& path);
				unsigned getLastSize() const {return size;}
				unsigned getSize() const {return rsize;}
				unsigned getCount() const {return header.size();}

				const std::string& getFileName(unsigned num);
			};
		}
	}
