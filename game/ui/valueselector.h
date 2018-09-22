/*
 * valueselector.h
 *
 *  Created on: 22 wrz 2018
 *      Author: crm
 */

#pragma once

#include "../../engine/graphics/ui/text.h"

#include "panel.h"
#include "button.h"

namespace Game
	{
	namespace UI
		{
		class ValueSelector: public UI::Panel
			{
			protected:
				Engine::Graphics::UI::Text text;

				std::string baseText;

				int value;
				int valueMin;
				int valueMax;

				int prevValue;

				Button* btnDecrease;
				Button* btnIncrease;

			public:
				ValueSelector(int w, int h, const std::string& text, int min, int max, int start);
				virtual ~ValueSelector();

				virtual void update(float dt);
				virtual void print(float tinterp);
				//virtual void clear();

				virtual bool click(const Engine::Math::VectorI& position);
				//virtual bool hover(const Engine::Math::VectorI& position);

				int getValue() const {return value;}
			};

		} /* namespace State */
	} /* namespace Game */
