/*
 * window.h
 *
 *  Created on: 30 lip 2018
 *      Author: crm
 */

#pragma once

#include "panel.h"

#include "../../engine/graphics/spriteptr.h"

namespace Game
	{
	namespace UI
		{
		class Window: public Panel
			{
			protected:
				bool resizeBackground;
				Engine::Graphics::SpritePtr background;

			public:
				Window(const Engine::Math::VectorI& size, const Engine::Graphics::SpritePtr& background, bool resizeBackground=false):
					Panel(size), resizeBackground(resizeBackground), background(background)
					{
					selectable=false;
					}
				virtual ~Window()
					{
					//
					}

				virtual void update(float dt) override;
				virtual void print(float tinterp) override;
				//virtual void clear();

				//virtual bool click(const Engine::Math::VectorI& position);
				//virtual bool hover(const Engine::Math::VectorI& position);
			};
		} /* namespace UI */
	} /* namespace Game */
