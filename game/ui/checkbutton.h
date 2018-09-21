/*
 * checkbutton.h
 *
 *  Created on: 20 wrz 2018
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
		class CheckButton: public UI::Panel
			{
			protected:
				Engine::Graphics::SpritePtr sprite;
				Engine::Graphics::UI::Text text;

				bool checked;
				bool wasHovered;

			public:
				CheckButton(int w, int h, const std::string& text, bool checked=false):
					Panel({w, h}), sprite(), text("font/dejavu.xml", text, w, h),
					checked(checked), wasHovered(false)
					{
					this->text.setAlignCenter();
					this->text.setAlignMiddle();
					this->text.update();
					}
				virtual ~CheckButton()
					{
					//
					}

				virtual void update(float dt);
				virtual void print(float tinterp);
				//virtual void clear();

				virtual bool click(const Engine::Math::VectorI& position);
				virtual bool hover(const Engine::Math::VectorI& position);

				bool isChecked() const {return checked;}
				void check() {checked=true;}
				void uncheck() {checked=false;}
				void setChecked(bool s) {checked=s;}
			};

		} /* namespace UI */
	} /* namespace Game */
