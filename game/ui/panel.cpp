/*
 * panel.cpp
 *
 *  Created on: 30 lip 2018
 *      Author: crm
 */

#include "panel.h"

#include "../../engine/debug/log.h"

using namespace Game::UI;

void Panel::update(float dt)
	{
	for(auto panel: children)
		{
		panel->update(dt);
		}
	}

void Panel::print(float tinterp)
	{
	for(auto panel: children)
		{
		panel->print(tinterp);
		}
	}

void Panel::clear()
	{
	for(auto panel: children)
		{
		delete panel;
		}

	children.clear();
	}

bool Panel::click(const Engine::Math::VectorI& position)
	{
	LOG_DEBUG("pnl %p click %d,%d children: %d", this, position.x, position.y, children.size());

	if(!hitCheck(position))
		return false;

	for(auto panel: children)
		{
		//if(!panel->hitCheck(position))
		//	continue;
		if(panel->click(position))
			return true;
		}

	return false;
	}

bool Panel::hover(const Engine::Math::VectorI& position)
	{
	for(auto panel: children)
		{
		if(panel->hover(position))
			return true;
		}

	return false;
	}


bool Panel::hasSelectedChild()
	{
	for(auto panel: children)
		{
		if(panel->isSelected())
			{
			return true;
			}

		if(panel->hasSelectedChild())
			{
			return true;
			}
		}
	return false;
	}

bool Panel::selectNext()
	{
	auto it=children.begin();

	if(selected)
		{
		selected=false;

		for(/**/; it!=children.end(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectNext())
				{
				return true;
				}
			}

		return false;
		}
	else if(hasSelectedChild())
		{
		for(/**/; it!=children.end(); ++it)
			{
			Panel* panel=*it;

			if(panel->isSelected())
				{
				break;
				}

			if(panel->hasSelectedChild())
				{
				if(panel->selectNext())
					{
					return true;
					}

				++it;
				break;
				}
			}

		for(/**/; it!=children.end(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectNext())
				{
				return true;
				}
			}

		if(isSelectable())
			{
			selected=true;
			return true;
			}
		}
	else
		{
		if(isSelectable())
			{
			selected=true;
			return true;
			}

		for(/**/; it!=children.end(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectNext())
				{
				return true;
				}
			}
		}

	return false;
	}

bool Panel::selectPrevious()
	{
	auto it=children.rbegin();

	if(selected)
		{
		selected=false;

		for(/**/; it!=children.rend(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectPrevious())
				{
				return true;
				}
			}

		return false;
		}
	else if(hasSelectedChild())
		{
		for(/**/; it!=children.rend(); ++it)
			{
			Panel* panel=*it;

			if(panel->isSelected())
				{
				break;
				}

			if(panel->hasSelectedChild())
				{
				if(panel->selectPrevious())
					{
					return true;
					}

				++it;
				break;
				}
			}

		for(/**/; it!=children.rend(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectPrevious())
				{
				return true;
				}
			}

		if(isSelectable())
			{
			selected=true;
			return true;
			}
		}
	else
		{
		if(isSelectable())
			{
			selected=true;
			return true;
			}

		for(/**/; it!=children.rend(); ++it)
			{
			Panel* panel=*it;

			if(panel->selectPrevious())
				{
				return true;
				}
			}
		}

	return false;
	}


void Panel::addChild(Panel* panel, const Engine::Math::VectorI& position, bool selectable)
	{
	panel->setPosition(position);
	panel->selectable=selectable;
	children.push_back(panel);
	}


void Panel::dump(int level)
	{
	LOG_DEBUG("%*s %p [%4d %4d][enabled %d][selected %d/%d]", level*4, ">", this, position.x, position.y, enabled, selected, selectable);

	for(auto panel: children)
		{
		panel->dump(level+1);
		}
	}



bool Panel::hitCheck(const Engine::Math::VectorI& position) const
	{
	return !(position.x < this->position.x ||
	         position.y < this->position.y ||
		     position.x > this->position.x+this->size.x ||
		     position.y > this->position.y+this->size.y);
	}
