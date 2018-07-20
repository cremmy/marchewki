/*
 * stringparser.h
 *
 *  Created on: 23-05-2014
 *      Author: crm
 */

#pragma once

#include <vector>
#include <string>

namespace Engine
	{
	namespace Utils
		{
		typedef std::string String;

		const String DEFAULT_DELIMITER=" \t";

		class StringParser
			{
			public:
				enum class DelimiterMode
					{
					EACH, // 1..2 dzielone po kropce podzieli na 3 elementy
					COMBINED, // 1..2 dzielone po kropce podzieli na 2 elementy
					};

			protected:
				struct Element
					{
					Element(): start(0), length(0) {}
					Element(const Element& e): start(e.start), length(e.length) {}

					unsigned start;
					unsigned length;
					};

				struct Iterator
					{
					protected:
						const StringParser* sp;
						unsigned i;

					public:
						Iterator(const StringParser* sp, unsigned i=0u): sp(sp), i(i) {}
						Iterator(const Iterator& it): sp(it.sp), i(it.i) {}

						Iterator& operator=(const Iterator& it) {sp=it.sp; i=it.i; return *this;}

						String operator*() {return sp->get(i);}
						void operator++() {++i;}
						bool operator!=(const Iterator& it) {return sp!=it.sp && i!=it.i;}
					};

				std::vector<Element> elements;

				String text;
				String delimiters;

				DelimiterMode dmode;

				void parse();
				bool isDelimiter(String::value_type c) const;

			public:
				StringParser(DelimiterMode dmode=DelimiterMode::COMBINED);
				StringParser(const StringParser& sp);
				StringParser(const String& text, const String& delimiters=DEFAULT_DELIMITER, DelimiterMode dmode=DelimiterMode::COMBINED);

				StringParser& operator=(const String& s) {set(s); return *this;}
				String operator[](unsigned position) {return get(position);}
				String operator[](unsigned position) const {return get(position);}

				void clear();
				unsigned count() const {return elements.size();}

				void set(const String& s);
				void set(unsigned position, const String& s);
				void insert(const String& s);
				void insert(unsigned position, const String& s);
				void remove(unsigned position);
				void swap(unsigned p1, unsigned p2);

				String get() const {return text;}
				String get(unsigned position) const;
				int toInt(unsigned position) const;
				float toFloat(unsigned position) const;

				Iterator begin() const {return Iterator(this, 0u);}
				Iterator end() const {return Iterator(this, count());}
			};
		}
	}
