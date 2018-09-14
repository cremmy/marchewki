/*
 * button.h
 *
 *  Created on: 31 lip 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/graphics/spriteptr.h"
#include "../../engine/graphics/ui/text.h"
#include "panel.h"

namespace Game
	{
	namespace UI
		{
		class Button: public Panel
			{
			protected:
				Engine::Graphics::SpritePtr sprite;

				int* receiver;
				int receiverCode;

				bool wasClicked;
				bool wasHovered;

				Engine::Graphics::SpritePtr hoverBackground;
				Engine::Graphics::UI::Text hoverMessage;
				Engine::Math::VectorI hoverPosition;

			public:
				Button(const Engine::Graphics::SpritePtr& sprite, int* receiver, int code):
					Panel({sprite.getCurrentFrame().getWidth(), sprite.getCurrentFrame().getHeight()}), sprite(sprite),
					receiver(receiver), receiverCode(code),
					wasClicked(false), wasHovered(false)
					{
					//
					}
				virtual ~Button()
					{
					//
					}

				virtual void update(float dt);
				virtual void print(float tinterp);
				//virtual void clear();

				virtual bool click(const Engine::Math::VectorI& position);
				virtual bool hover(const Engine::Math::VectorI& position);

				void setHoverMessage(const std::string& text);
			};

		} /* namespace UI */
	} /* namespace Game */
