/*
 * log.cpp
 *
 *  Created on: 26-10-2016
 *      Author: crm
 */

#include "log.h"

#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#ifdef UTILS_WINDOWS_LOG_COLORS
#include <windows.h>
#endif

#undef ERROR

// TODO Zamiana na singletona (istniejące makra mi ułatwią przejście)
// TODO pre-printowanie do tablicy, podział na linie

namespace Engine
	{
	namespace Debug
		{
		namespace Log
			{
			void date();
			void flush();

			void lock();
			void unlock();

			/*int length=80;*/
			int level=ALL;
			bool llocation=true;
			int day=0;

			FILE *out=stdout;

			std::mutex mutex;

			const char LOCATION_FORMAT[]="[%-64s]";

			/*unsigned countError=0u;
			unsigned countWarning=0u;
			unsigned countInfo=0u;
			unsigned countSuccess=0u;
			unsigned countDebug=0u;*/

#ifdef UTILS_WINDOWS_LOG_COLORS
			HANDLE cHandle; // GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO cInfo;

			unsigned colors[5];
#endif
			}
		}
	}

bool Engine::Debug::Log::init(/*int linelength*/)
	{
#ifdef UTILS_WINDOWS_LOG_COLORS
	setTypeColor(ERROR,   Color::RED,        Color::BLACK);
	setTypeColor(WARNING, Color::YELLOW,     Color::BLACK);
	setTypeColor(INFO,    Color::WHITE,      Color::BLACK);
	setTypeColor(SUCCESS, Color::GREEN,      Color::BLACK);
	setTypeColor(DEBUG,   Color::LIGHT_CYAN, Color::DARK_CYAN);

	cHandle=GetStdHandle(STD_OUTPUT_HANDLE);

	if(!cHandle)
		return false;

	if(!GetConsoleScreenBufferInfo(cHandle, &cInfo))
		{
		GetLastError();

		return false;
		}
#endif

	return true;
	}

void Engine::Debug::Log::clear()
	{
	lock();
	if(out!=nullptr && out!=stdout)
		fclose(out);

#ifdef UTILS_WINDOWS_LOG_COLORS
	if(cHandle)
		SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	unlock();
	}

void Engine::Debug::Log::lock()
	{
	mutex.lock();
	}

void Engine::Debug::Log::unlock()
	{
	mutex.unlock();
	}

void Engine::Debug::Log::date()
	{
	static char date[64];
	static std::chrono::system_clock::time_point now;
	static std::time_t nowt;
	static std::tm *nowtm;

	now=std::chrono::system_clock::now();
	nowt=std::chrono::system_clock::to_time_t(now);
	nowtm=std::localtime(&nowt);

	if(day!=nowtm->tm_mday)
		{
		strftime(date, 64, "%Y-%m-%d", nowtm);
		day=nowtm->tm_mday;

		fprintf(out, "[%s]\n", date);
		}

	strftime(date, 64, "%H:%M:%S", nowtm);
	fprintf(out, "[%s.%03d]", date, (int)(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()%1000));
	}

void Engine::Debug::Log::flush()
	{
	fflush(out);
	}


void Engine::Debug::Log::error(const char *location, const char *fmt, ...)
	{
	if(!(level&ERROR))
		return;

	lock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	date();
	if(llocation)
		fprintf(out, LOCATION_FORMAT, location);

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, colors[0]);
#endif

	fprintf(out, " ERR : ");

	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);

	fprintf(out, "\n");
	flush();
	unlock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif
	}

void Engine::Debug::Log::warning(const char *location, const char *fmt, ...)
	{
	if(!(level&WARNING))
		return;

	lock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	date();
	if(llocation)
		fprintf(out, LOCATION_FORMAT, location);

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, colors[1]);
#endif

	fprintf(out, " WARN: ");

	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);

	fprintf(out, "\n");
	flush();
	unlock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif
	}

void Engine::Debug::Log::info(const char *location, const char *fmt, ...)
	{
	if(!(level&INFO))
		return;

	lock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	date();
	if(llocation)
		fprintf(out, LOCATION_FORMAT, location);

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, colors[2]);
#endif

	fprintf(out, " INFO: ");

	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);

	fprintf(out, "\n");
	flush();
	unlock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif
	}

void Engine::Debug::Log::success(const char *location, const char *fmt, ...)
	{
	if(!(level&SUCCESS))
		return;

	lock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	date();
	if(llocation)
		fprintf(out, LOCATION_FORMAT, location);

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, colors[3]);
#endif

	fprintf(out, " OK  : ");

	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);

	fprintf(out, "\n");
	flush();
	unlock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif
	}

void Engine::Debug::Log::debug(const char *location, const char *fmt, ...)
	{
	if(!(level&DEBUG))
		return;

	lock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif

	date();
	if(llocation)
		fprintf(out, LOCATION_FORMAT, location);

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, colors[4]);
#endif

	fprintf(out, " DBG : ");

	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);

	fprintf(out, "\n");
	flush();
	unlock();

#ifdef UTILS_WINDOWS_LOG_COLORS
	SetConsoleTextAttribute(cHandle, cInfo.wAttributes);
#endif
	}


void Engine::Debug::Log::setOut(FILE *sf)
	{
	if(!sf)
		return;

	lock();
	out=sf;
	unlock();
	}

void Engine::Debug::Log::setLevel(int flag)
	{
	level=flag;
	}

void Engine::Debug::Log::setPrintLocation(bool enabled)
	{
	llocation=enabled;
	}

#ifdef UTILS_WINDOWS_LOG_COLORS
void Engine::Debug::Log::setTypeColor(Engine::Debug::Log::Level level, Engine::Debug::Log::Color foreground, Engine::Debug::Log::Color background)
	{
	const unsigned nc=((unsigned char)foreground)|((unsigned char)background<<4);

	switch(level)
		{
		case ERROR:   colors[0]=nc; break;
		case WARNING: colors[1]=nc; break;
		case INFO:    colors[2]=nc; break;
		case SUCCESS: colors[3]=nc; break;
		case DEBUG:   colors[4]=nc; break;
		default:
			//
		break;
		}
	}
#endif

void Engine::Debug::Log::stats()
	{
	LOG_INFO("Nope, nic tu nie ma");
	}
