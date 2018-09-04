/*
 * panel.h
 *
 *  Created on: 30 lip 2018
 *      Author: crm
 */

#pragma once

#include <vector>
#include "../../engine/math/math.h"

namespace Game
	{
	namespace UI
		{
		class Panel
			{
			protected:
				Engine::Math::VectorI position;
				Engine::Math::VectorI size;

				bool enabled;

				// bool canHasChildren; // Lolspeak
				std::vector<Panel*> children;

				bool selectable;
				bool selected;
				bool hasSelectedChild();

				bool hitCheck(const Engine::Math::VectorI& position) const;

			public:
				Panel(const Engine::Math::VectorI& size):
					position(), size(size), enabled(true),
					selectable(false), selected(false)
					{
					//
					}
				virtual ~Panel()
					{
					//
					}

				virtual void update(float dt);
				virtual void print(float tinterp);
				virtual void clear();

				virtual bool click(const Engine::Math::VectorI& position);
				virtual bool hover(const Engine::Math::VectorI& position);

				virtual bool selectNext();
				virtual bool selectPrevious();

				void addChild(Panel* panel, const Engine::Math::VectorI& position, bool selectable=false);

				Engine::Math::VectorI getPosition() const {return position;}
				Engine::Math::VectorI getSize() const {return size;}
				bool isEnabled() const {return enabled;}
				bool isSelectable() const {return selectable;}
				bool isSelected() const {return selected;}

				void setPosition(const Engine::Math::VectorI& s) {position=s;}
				void setSize(const Engine::Math::VectorI& s) {size=s;}
				void enable() {enabled=true;}
				void disable() {enabled=false;}

				void dump(int level=0);
			};

		} /* namespace UI */
	} /* namespace Game */
