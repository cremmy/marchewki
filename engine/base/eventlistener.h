/*
 * eventlistener.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <deque>

namespace Engine
	{
	namespace Base
		{
		template <typename Event>
		class EventListener
			{
			private:
				std::deque<Event> events;

			protected:
				bool popEvent(Event& e)
					{
					if(events.empty())
						return false;

					e=events.front();
					events.pop_front();

					return true;
					}

			public:
				EventListener() {}
				virtual ~EventListener() {events.clear();}

				void pushEvent(const Event& e) {events.push_back(e);}
			};
		} /* namespace Base */
	} /* namespace Engine */
