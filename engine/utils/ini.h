/*
 * Ini.h
 *
 *  Created on: 18-10-2011
 *      Author: crm
 */

#pragma once

#include <string>
#include <map>

namespace Engine
	{
	namespace Utils
		{

		/***
		 * \class Ini
		 * \brief Prosta obsluga plikow ini
		 */
		class Ini
			{
			private:
				std::map<std::string, std::string> vals;
				std::map<std::string, std::string>::iterator it;

			public:
				 Ini() {it=vals.end();}
				~Ini() {clear();}
				/***
				 * \brief Wczytuje grupe \a group z pliku \a fileName
				 */
				bool open(const std::string& fileName, const std::string& group);
				void clear();

				bool save(const std::string& fileName, const std::string& group);

				/***
				 * \brief Wpisuje \a key do zmiennej \val
				 */
				void get(const std::string& key, std::string& val);
				std::string get(const std::string& key);

				void set(const std::string& key, const std::string& val);

				/**
				 * \brief Wpisuje pierwsza wartosc do \val
				 * \return \a true jesli cos jeszcze jest, false w przeciwnym razie
				 */
				bool getFirst(std::string& key, std::string& val);
				/**
				 * \brief Wpisuje nastepna wartosc po ostatniej do \val
				 * \return \a true jesli cos jeszcze jest, false w przeciwnym razie
				 */
				bool getNext(std::string& key, std::string& val);
			};
		}
	}

