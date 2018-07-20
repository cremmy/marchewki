/*
 * Ini.cpp
 *
 *  Created on: 18-10-2011
 *      Author: crm
 */

#include <fstream>

#include "ini.h"

using namespace Engine::Utils;
using namespace std;

bool Ini::open(const string& fileName, const string& group)
	{
	//printf("> %s - %s\n", fileName.c_str(), group.c_str());
	fstream in(fileName.c_str(), ios::in );
	if(!in.is_open())
		return false;
	clear();		// Przeniesione na poczatek funkcji celem unikniecia zapetlenia npc

	bool read=false;
	std::string tmp;
	while(in)
		{
		//in.getline(tmp, 1024);
		std::getline(in, tmp, '\n');

		if(tmp[0]==';')
			continue;
		if(!read)
			{
			if(tmp[0]=='[')
				{
				unsigned len;
				for(len=1; len<tmp.length(); ++len)
					{
					if(tmp[len]==']')
						break;
					}
				if(group.compare( tmp.substr(1, len-1) )==0)
					read=1;
				}
			}
		else
			{
			if(tmp[0]=='[')
				{
				/*unsigned len;
				for(len=1; len<tmp.length(); ++len)
					{
					if(tmp[len]==']')
						break;
					}
				if(group.compare( tmp.substr(1, len-1) )!=0)
					read=0;
				continue;*/
				break;
				}
			int eqpos;		// pozycja '='
			for(eqpos=1; eqpos<1024; ++eqpos)
				{
				if(tmp[eqpos]=='=')
					break;
				}
			if(eqpos >= (int)tmp.length() || tmp.empty() )//tmp.length()<1)
				continue;

			vals[tmp.substr(0, eqpos)]=tmp.substr(eqpos+1, tmp.length()-eqpos-1);
			}
		}

	in.close();

	return true;
	}

void Ini::clear()
	{
	vals.clear();
	}

bool Ini::save(const std::string& fileName, const std::string& group)
	{
	std::fstream out;

	out.open(fileName.c_str(), std::ios::out);
	out << group << "\n";
	for(std::map<std::string, std::string>::iterator it=vals.begin(); it!=vals.end(); ++it)
		out << it->first << "=" << it->second << "\n";
	out.close();

	return !out.fail();
	}

void Ini::get(const string& key, string& val)
	{
	val=vals[key];
	}

void Ini::set(const std::string& key, const std::string& val)
	{
	vals[key]=val;
	}

std::string Ini::get(const std::string& key)
	{
	return vals[key];
	}

bool Ini::getFirst(std::string& key, std::string& val)
	{
	it=vals.begin();
	if(it==vals.end())
		return false;

	key=it->first;
	val=it->second;
	return !false;
	}

bool Ini::getNext(std::string& key, std::string& val)
	{
	if(it==vals.end())
		return false;

	++it;
	if(it==vals.end())
		return false;

	key=it->first;
	val=it->second;
	return !false;
	}
