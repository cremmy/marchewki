/*
 * assert.h
 *
 *  Created on: 19 kwi 2017
 *      Author: crm
 */

#pragma once

#include <cstdio>
#include <cstdlib>
//#include <assert.h>

#include "log.h"

#undef assert

namespace Engine
	{
	namespace Debug
		{
		#if defined(BUILD_RELEASE)
			#define assert(e) (void(0))
		#elif !defined(BUILD_DEBUG)
			#define assert(e) (void(0))
		#else
			#define assert(e) ((e)?((void)0):(Engine::Debug::_assertFired(#e, (__FILE__ ":" __LOG_STR_2(__LINE__)))))

			inline void _assertFired(const char *condition, const char *location)
				{
				Engine::Debug::Log::error(location, "ASSERT FAILED: %s", condition);
				exit(1);
				}
		#endif
		}
	}


