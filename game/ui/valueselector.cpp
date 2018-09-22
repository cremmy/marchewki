/*
 * valueselector.cpp
 *
 *  Created on: 22 wrz 2018
 *      Author: crm
 */

#include "valueselector.h"

#include <sstream>

#include "../../engine/debug/log.h"
#include "../../engine/render/render.h"

using namespace Game::UI;

ValueSelector::ValueSelector(int w, int h, const std::string& text, int min, int max, int start):
	Panel({w, h}), text("font/dejavu.xml", "", w, h), baseText(text),
	value(start), valueMin(min), valueMax(max), prevValue(start-1),
	btnDecrease(nullptr), btnIncrease(nullptr)
	{
	this->text.setAlignCenter();
	this->text.setAlignMiddle();

	btnDecrease=new Button(h, h, "[-]", nullptr, 0);
	btnIncrease=new Button(h, h, "[+]", nullptr, 0);

	addChild(btnDecrease, {0, 0}, true);
	addChild(btnIncrease, {0, 0}, true);
	}
ValueSelector::~ValueSelector()
	{
	//
	}


void ValueSelector::update(float dt)
	{
	using namespace Engine::Math;

	if(value!=prevValue)
		{
		prevValue=value;

		std::stringstream ss;
		ss << baseText << ": " << value;

		text.setStr(ss.str());
		text.update();
		}

	btnDecrease->setPosition(position+VectorI((size.x-text.getRealW())/2, 0));
	btnIncrease->setPosition(position+VectorI((size.x+text.getRealW())/2-btnIncrease->getSize().x, 0));
	}

void ValueSelector::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	if(!enabled)
		{
		Render::getInstance().setColor(Vector(0.5f, 0.5f, 0.5f, 1.0f));
		}

	if(!text.isEmpty())
		text.print(Orientation::GUI+Vector(position.x, position.y));

	Panel::print(tinterp);
	Render::getInstance().setColor(Vector(1, 1, 1, 1));
	}


bool ValueSelector::click(const Engine::Math::VectorI& position)
	{
	if(!hitCheck(position))
		return false;
	if(!enabled)
		return true;

	if(btnDecrease->click(position))
		{
		--value;
		if(value<valueMin)
			value=valueMin;
		return true;
		}
	else if(btnIncrease->click(position))
		{
		++value;
		if(value>valueMax)
			value=valueMax;
		return true;
		}

	return true;
	}
