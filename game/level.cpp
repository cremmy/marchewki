/*
 * level.cpp
 *
 *  Created on: 22 lip 2018
 *      Author: crm
 */

#include "level.h"

using namespace Game;

bool Level::init(unsigned w, unsigned h)
	{
	clear();

	return resize(w, h);
	}

void Level::update(float dt)
	{
	//
	}

void Level::print(float tinterp)
	{
	//
	}

void Level::clear()
	{
	for(unsigned y=0u; y<field.size(); ++y)
		{
		for(unsigned x=0u; x<field[y].size(); ++x)
			{
			delete field[y][x];
			}

		field[y].clear();
		}

	field.clear();
	}


bool Level::resize(unsigned w, unsigned h)
	{
	const unsigned OLD_H=field.size();
	const unsigned OLD_W=field.empty()?0u:field[0u].size();

	// Pomniejszanie, skasuj nadmiarowe pola
	if(h<OLD_H)
		{
		for(unsigned y=h; y<OLD_H; ++y)
			{
			for(unsigned x=0u; x<field[y].size(); ++x)
				{
				delete field[y][x];
				}
			field[y].clear();
			}
		}

	field.resize(h);

	if(h>OLD_H)
		{
		for(unsigned y=OLD_H; y<h; ++y)
			{
			for(unsigned x=0u; x<field[y].size(); ++x)
				{
				field[y][x]=nullptr;
				}
			}
		}

	// TODO zmiana wielkości w X
	}

bool Level::resizeIncreaseXByOne()
	{
	return resize(field.empty()?1u:field[0u].size()+1u, field.size());
	}

bool Level::resizeIncreaseYByOne()
	{
	return resize(field.empty()?1u:field[0u].size(), field.size()+1u);
	}


Level::Field* Level::getField(unsigned x, unsigned y)
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}

const Level::Field* Level::getField(unsigned x, unsigned y) const
	{
	if(x>=getWidth() || y>=getHeight())
		return nullptr;
	return field[y][x];
	}
