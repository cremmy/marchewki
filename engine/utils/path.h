/*
 * path.h
 *
 *  Created on: 4 sty 2018
 *      Author: crm
 */

#pragma once

#include <string>

namespace Engine
	{
	namespace Utils
		{
		std::string getRealPath(const std::string& base, const std::string& path);
		}
	}


