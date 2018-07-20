/*
 * event.h
 *
 *  Created on: 4 lip 2017
 *      Author: crm
 */

#pragma once

namespace Engine
	{
	namespace Core
		{
		struct AppEvent
			{
			public:
				enum class Type
					{
					NONE,

					KEY_DOWN,
					KEY_UP,
					MOUSE_MOVE,
					MOUSE_KEY_DOWN,
					MOUSE_KEY_UP,
					MOUSE_WHEEL,
					WINDOW_HIDE,
					WINDOW_SHOW,
					};

			protected:
				Type type;

			public:
				union
					{
					struct
						{
						unsigned key;
						unsigned modifiers;
						} keyboard;
					struct
						{
						int x;
						int y;
						unsigned key;
						} mouse;
					} data;

			public:
				AppEvent(): type(Type::NONE), data() {}
				AppEvent(Type type): type(type), data() {}
				AppEvent(const AppEvent& e): type(e.type), data(e.data) {}
				~AppEvent() {}

				const AppEvent& initKeyDown(unsigned key, unsigned modifiers=0u)
					{
					type=Type::KEY_DOWN;
					data.keyboard.key=key;
					data.keyboard.modifiers=modifiers;
					return *this;
					}
				const AppEvent& initKeyUp(unsigned key, unsigned modifiers=0u)
					{
					type=Type::KEY_UP;
					data.keyboard.key=key;
					data.keyboard.modifiers=modifiers;
					return *this;
					}
				const AppEvent& initMouseMove(int x, int y, unsigned key)
					{
					type=Type::MOUSE_MOVE;
					data.mouse.x=x;
					data.mouse.y=y;
					data.mouse.key=key;
					return *this;
					}
				const AppEvent& initMouseKeyDown(int x, int y, unsigned key)
					{
					type=Type::MOUSE_KEY_DOWN;
					data.mouse.x=x;
					data.mouse.y=y;
					data.mouse.key=key;
					return *this;
					}
				const AppEvent& initMouseKeyUp(int x, int y, unsigned key)
					{
					type=Type::MOUSE_KEY_UP;
					data.mouse.x=x;
					data.mouse.y=y;
					data.mouse.key=key;
					return *this;
					}
				const AppEvent& initMouseWheel(int direction)
					{
					type=Type::MOUSE_WHEEL;
					data.mouse.x=0;
					data.mouse.y=direction;
					data.mouse.key=0;
					return *this;
					}
				const AppEvent& initWindowHide()
					{
					type=Type::WINDOW_HIDE;
					return *this;
					}
				const AppEvent& initWindowShow()
					{
					type=Type::WINDOW_SHOW;
					return *this;
					}

				Type getType() const {return type;}
			};
		} /* namespace Core */
	} /* namespace Engine */
