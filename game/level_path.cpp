/*
 * level_path.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "level.h"

#include <algorithm>

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "turret.h"

using namespace Game;

bool Level::refreshPath()
	{
	const unsigned WIDTH=getWidth();
	const unsigned HEIGHT=getHeight();
	const int DISTANCE_INF=0x7FFF;

	if(WIDTH<1u || HEIGHT<1u)
		return false;

	// Powiększanie grafu
	if(nodes.size()!=field.size() || (nodes.size()>0u && nodes[0u].size()>field[0u].size()))
		{
		nodes.resize(field.size());
		for(unsigned y=0u; y<nodes.size(); ++y)
			{
			nodes[y].resize(field[y].size());

			for(unsigned x=0u; x<nodes[y].size(); ++x)
				{
				nodes[y][x].x=x;
				nodes[y][x].y=y;
				}
			}
		}

	auto compareNodes=[](const GraphNode* a, const GraphNode* b)->bool
		{
		return a->distance>b->distance;
		};

	auto getNeighbour=[this, WIDTH, HEIGHT](GraphNode* node, int side)->GraphNode*
		{
		switch(side)
			{
			case GraphNode::NEIGHBOUR_UP:
				return (node->y<(int)HEIGHT-1)?&nodes[node->y+1][node->x]:nullptr;
			case GraphNode::NEIGHBOUR_LEFT:
				return (node->x>0)?&nodes[node->y][node->x-1]:nullptr;
			case GraphNode::NEIGHBOUR_DOWN:
				return (node->y>0)?&nodes[node->y-1][node->x]:nullptr;
			case GraphNode::NEIGHBOUR_RIGHT:
				return (node->x<(int)WIDTH-1)?&nodes[node->y][node->x+1]:nullptr;
			default:
				return nullptr;
			}
		};

	auto getDistance=[](int sx, int sy)->int
		{
		// Baza zawsze jest w 0,0
		return abs(sx)+abs(sy);
		};

	std::vector<GraphNode*> open;
	open.reserve(WIDTH*HEIGHT);

	GraphNode& origin=nodes[0u][0u];
	origin.distance=0;

	for(auto& row: nodes)
		{
		for(auto& node: row)
			{
			if(&node!=&origin)
				node.distance=DISTANCE_INF; // Infinity?
			node.prev=nullptr;

			open.push_back(&node);
			}
		}

	while(!open.empty())
		{
		std::sort(open.begin(), open.end(), compareNodes);
		GraphNode* node=open.back();

		open.pop_back();

		for(int i=0; i<4; ++i)
			{
			GraphNode* neighbour=getNeighbour(node, i);

			if(!neighbour)
				continue;

			const Field* field=getField({neighbour->x, neighbour->y});

			if(field->turret && !field->turret->isWalkable())
				{
				continue;
				}

			const int alt=node->distance+getDistance(neighbour->x, neighbour->y);

			if(alt<neighbour->distance)
				{
				neighbour->distance=alt;
				neighbour->prev=node;
				}
			}
		}

	// TODO Sprawdzic czy nie ma miejsc z ktorych nie da sie dojsc do bazy
	// TODO Zamiast sprawdzać spójnośc, proponuję wyznaczyć ścieżkę
	/*for(auto& row: nodes)
		{
		for(auto& node: row)
			{
			const Field* field=getField({node.x, node.y});

			if(field->turret && !field->turret->isWalkable())
				{
				continue;
				}

			if(node.distance!=DISTANCE_INF)
				continue;

			LOG_WARNING("Graf niespojny: %d,%d", node.x, node.y);

			return false;
			}
		}*/
	for(int y=0; y<getHeight(); ++y)
		{
		for(int x=0; x<getWidth(); ++x)
			{
			Field* field=getField({x, y});

			if(!field->turret)
				{
				continue;
				}
			if(field->turret->getType()!=TurretType::ENEMY_SPAWNER)
				{
				continue;
				}

			GraphNode* node=&nodes[y][x];
			for(int i=0; i<getWidth()*getHeight(); ++i)
				{
				if(!node)
					return false;

				if(node->x==0 && node->y==0)
					break;

				node=node->prev;
				}
			}
		}

	++pathVersion;

	return true;
	}

Engine::Math::VectorI Level::findPath(const Engine::Math::VectorI& from)
	{
	if(!getField(from))
		return from;

	GraphNode& node=nodes[from.y][from.x];

	if(!node.prev)
		{
		return from;
		}

	return Engine::Math::VectorI(node.prev->x, node.prev->y);
	}

Engine::Math::Vector Level::findPath(const Engine::Math::Vector& from)
	{
	const Engine::Math::VectorI posfield=getPositionOnField(from);
	const Engine::Math::VectorI next=findPath(posfield);

	return getFieldPosition(next);
	}

void Level::findPath(std::list<Engine::Math::VectorI>& path, const Engine::Math::Vector& from)
	{
	const Engine::Math::VectorI fposition=getPositionOnField(from);

	path.clear();

	if(!getField(fposition))
		{
		return;
		}

	GraphNode* node=&nodes[fposition.y][fposition.x];

	while(node)
		{
		path.push_back({node->x, node->y});

		node=node->prev;
		}

	if(path.back().x!=0 || path.back().y!=0)
		{
		LOG_WARNING("Nie znaleziono sciezki");
		//path.clear();
		}
	}
