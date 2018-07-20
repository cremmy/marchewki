/*
 * eventprovider.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <vector>

#include "../debug/assert.h"

namespace Engine
	{
	namespace Base
		{
		template <typename Event>
		class EventListener;

		template <typename Event>
		class EventProvider
			{
			private:
				std::vector<EventListener<Event>*> listeners;

			protected:
				void sendEvent(const Event& event)
					{
					for(auto listener: listeners)
						{
						listener->pushEvent(event);
						}
					}

			public:
				EventProvider() {}
				virtual ~EventProvider() {listeners.clear();}

				void addListener(EventListener<Event>& listener)
					{
#ifdef BUILD_DEBUG
					for(auto* l: listeners)
						{
						assert(l!=&listener);
						}
#endif

					listeners.push_back(&listener);
					}

				void removeListener(EventListener<Event>& listener)
					{
					for(auto it=listeners.begin(); it!=listeners.end(); ++it)
						{
						if(*it==&listener)
							continue;

						listeners.erase(it--);
						}
					}
			};

		} /* namespace Base */
	} /* namespace Engine */
