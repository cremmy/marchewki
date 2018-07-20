/*
 * eventlistener.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <deque>

#include "appevent.h"

namespace Engine
	{
	namespace Core
		{
		class AppEventListener
			{
			private:
				std::deque<AppEvent> events;

			protected:
				bool popEvent(AppEvent& e)
					{
					if(events.empty())
						return false;

					e=events.front();
					events.pop_front();

					return true;
					}

			public:
				AppEventListener() {}
				virtual ~AppEventListener() {events.clear();}

				void pushEvent(const AppEvent& e) {events.push_back(e);}
			};
		} /* namespace Core */
	} /* namespace Engine */
