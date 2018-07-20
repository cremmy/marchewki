/*
 * path.cpp
 *
 *  Created on: 4 sty 2018
 *      Author: crm
 */

#include "path.h"

std::string Engine::Utils::getRealPath(const std::string& base, const std::string& path)
	{
	if(path.empty())
		return path;

	// Sciezka absolutna
	if(path[0]=='/')
		{
		return path.substr(1);
		}

	// Sciezka relatywna
	const auto slash=base.rfind('/');

	if(slash==base.npos)
		{
		return path;
		}

	return base.substr(0, slash)+'/'+path;
	}


