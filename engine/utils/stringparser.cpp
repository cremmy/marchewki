/*
 * stringparser.cpp
 *
 *  Created on: 23-05-2014
 *      Author: crm
 */

#include "stringparser.h"

#include <cstdio>
#include <sstream>

using namespace Engine::Utils;

StringParser::StringParser(DelimiterMode dmode):
	elements(), text(""), delimiters(DEFAULT_DELIMITER), dmode(dmode)
	{
	//
	}

StringParser::StringParser(const StringParser& sp):
	elements(sp.elements), text(sp.text), delimiters(sp.delimiters), dmode(sp.dmode)
	{
	if(this->delimiters.size()<1)
		this->delimiters=DEFAULT_DELIMITER;
	}

StringParser::StringParser(const String& text, const String& delimiters, DelimiterMode dmode):
	elements(), text(text), delimiters(delimiters), dmode(dmode)
	{
	if(this->delimiters.size()<1)
		this->delimiters=DEFAULT_DELIMITER;

	if(text.size()>0)
		parse();
	}

void StringParser::parse()
	{
	Element cur;
	cur.start=0;
	cur.length=1;

	elements.clear();
	//elements.reserve(20);

	bool gotelement=false;

	//const unsigned size=text.size();
	//for(unsigned i=0; i<size; ++i)
	const String::value_type *start=text.c_str();
	const String::value_type *ptr=start;

	for(/**/; *ptr; ++ptr)
		{
		//bool delim=isDelimiter(text[i]);
		bool delim=isDelimiter(*ptr);

		if(gotelement)
			{
			if(!delim)
				++cur.length;
			else
				{
				elements.push_back(cur);
				//printf("start: %d dlugosc: %d\n", cur.start, cur.length);
				gotelement=false;
				}
			}
		else if(!delim)
			{
			cur.start=ptr-start;
			cur.length=1;
			gotelement=true;
			}
		else if(dmode==DelimiterMode::EACH)
			{
			cur.start=ptr-start;
			cur.length=0;
			elements.push_back(cur);
			}
		}

	if(gotelement)
		elements.push_back(cur);
	}

bool StringParser::isDelimiter(String::value_type c) const
	{
	//const unsigned size=delimiters.size();
	//for(unsigned i=0u; i<size; ++i)
	//	if(c==delimiters[i])
	const String::value_type *ptr=delimiters.c_str();
	for(/**/; *ptr; ++ptr)
		if(*ptr==c)
			return true;
	return false;
	}

void StringParser::clear()
	{
	elements.clear();
	text.clear();
	delimiters=DEFAULT_DELIMITER;
	}


void StringParser::set(const String& s)
	{
	text=s;

	parse();
	}

void StringParser::set(unsigned position, const String& s)
	{
	if(position>=count())
		{
		insert(s);
		return;
		}

	const Element& e=elements[position];

	text=text.substr(0, e.start) + s + text.substr(e.start+e.length, text.length()-e.start-e.length);

	parse();
	}

void StringParser::insert(const String& s)
	{
	text.append(delimiters[0]+s);

	parse();
	}

void StringParser::insert(unsigned position, const String& s)
	{
	if(position>=count())
		{
		insert(s);
		return;
		}

	const Element& e=elements[position];

	text=text.substr(0, e.start) + s + delimiters[0] + text.substr(e.start, text.length()-e.start);

	parse();
	}

void StringParser::remove(unsigned position)
	{
	if(position>=count())
		{
		return;
		}
	if(position==count()-1u)
		{
		elements.pop_back();
		return;
		}

	const Element& e=elements[position];
	const Element& ne=elements[position+1u];

	const unsigned length=ne.start-e.start;

	text=text.substr(0, e.start) + text.substr(e.start+length, text.length()-e.start-length);

	for(unsigned i=position+1; i<count(); ++i)
		elements[i].start-=length;

	elements.erase(elements.begin()+position);
	//parse();
	}

void StringParser::swap(unsigned p1, unsigned p2)
	{
	if(p1>p2)
		{
		unsigned tmp=p1;
		p1=p2;
		p2=tmp;
		}
	if(p2>=count())
		{
		return;
		}

	const Element &e1=elements[p1];
	const Element &e2=elements[p2];

	text=text.substr(0, e1.start) +
		 text.substr(e2.start, e2.length) +
		 text.substr(e1.start+e1.length, e2.start-e1.start-e1.length) +
		 text.substr(e1.start, e1.length) +
		 text.substr(e2.start+e2.length, text.length()-e2.start-e2.length);

	parse();
	}


String StringParser::get(unsigned position) const
	{
	if(position>=count())
		return String();

	const Element& e=elements[position];

	return text.substr(e.start, e.length);
	}


int StringParser::toInt(unsigned position) const
	{
	if(position>=count())
		return 0;

	int ret;
	std::stringstream ss;

	ss << get(position);
	ss >> ret;

	return ret;
	}

float StringParser::toFloat(unsigned position) const
	{
	if(position>=count())
		return 0.0f;

	float ret;
	std::stringstream ss;

	ss << get(position);
	ss >> ret;

	return ret;
	}
