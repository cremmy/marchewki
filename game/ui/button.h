/*
 * button.h
 *
 *  Created on: 31 lip 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/graphics/spriteptr.h"
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

			public:
				Button(const Engine::Graphics::SpritePtr& sprite, int* receiver, int code):
					Panel({sprite.getCurrentFrame().getWidth(), sprite.getCurrentFrame().getHeight()}), sprite(sprite),
					receiver(nullptr), receiverCode(code),
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
			};

		} /* namespace UI */
	} /* namespace Game */
