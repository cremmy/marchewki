/*
 * log.h
 *
 *  Created on: 26-10-2016
 *      Author: crm
 */

#pragma once

#include <cstdio>

//#define printf LOG_INFO
#define __LOG_STR(x) #x
#define __LOG_STR_2(x) __LOG_STR(x)
#define LOG_ERROR(fmt, ...) Engine::Debug::Log::error((__FILE__ ":" __LOG_STR_2(__LINE__)), fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Engine::Debug::Log::warning((__FILE__ ":" __LOG_STR_2(__LINE__)), fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Engine::Debug::Log::info((__FILE__ ":" __LOG_STR_2(__LINE__)), fmt, ##__VA_ARGS__ )
#define LOG_SUCCESS(fmt, ...) Engine::Debug::Log::success((__FILE__ ":" __LOG_STR_2(__LINE__)), fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) Engine::Debug::Log::debug((__FILE__ ":" __LOG_STR_2(__LINE__)), fmt, ##__VA_ARGS__)

//#define UTILS_WINDOWS_LOG_COLORS

namespace Engine
	{
	namespace Debug
		{
		namespace Log
			{
			enum Level
				{
				NONE=      0x00,
				ERROR=     0x01,
				WARNING=   0x02,
				INFO=      0x04,
				SUCCESS=   0x08,
				DEBUG=     0x10,

				PRODUCTION=0x0F,
				ALL=       0xFF,
				};

#ifdef UTILS_WINDOWS_LOG_COLORS
			enum class Color: unsigned char
				{
				BLACK=0,
				DARK_BLUE=    0x1,
				DARK_GREEN=   0x2,
				DARK_CYAN=    0x3,
				DARK_RED=     0x4,
				DARK_MAGENTA= 0x5,
				DARK_YELLOW=  0x6,
				DARK_WHITE=   0x7,
				DARK_GRAY=    0x8,
				LIGHT_BLUE=   0x9,
				LIGHT_GREEN=  0xA,
				LIGHT_CYAN=   0xB,
				LIGHT_RED=    0xC,
				LIGHT_MAGENTA=0xD,
				LIGHT_YELLOW= 0xE,
				LIGHT_WHITE=  0xF,

				BLUE=LIGHT_BLUE,
				GREEN=LIGHT_GREEN,
				CYAN=LIGHT_CYAN,
				RED=LIGHT_RED,
				MAGENTA=LIGHT_MAGENTA,
				PINK=LIGHT_MAGENTA,
				YELLOW=LIGHT_YELLOW,
				WHITE=LIGHT_WHITE,
				DARK_PINK=DARK_MAGENTA,
				GRAY=DARK_WHITE,
				};
#endif

			bool init(/*int linelength=80*/);
			void clear();

			void error(const char *location, const char *fmt, ...);
			void warning(const char *location, const char *fmt, ...);
			void info(const char *location, const char *fmt, ...);
			void success(const char *location, const char *fmt, ...);
			void debug(const char *location, const char *fmt, ...);

#ifdef UTILS_WINDOWS_LOG_COLORS
			void setTypeColor(Level level, Color foreground, Color background);
#endif

			void setOut(FILE *sf);
			void setLevel(int flag);
			void setPrintLocation(bool enabled); // Czy wypisywać ścieżkę do pliku z którego został wywołany print?

			void stats();
			};
		}
	}
