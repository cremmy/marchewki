/*
 * level_path.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "level.h"

#include <algorithm>
#include <limits>

#include "../engine/debug/log.h"
#include "../engine/render/render.h"

#include "turret.h"

using namespace Game;

bool Level::refreshPath()
	{
	const unsigned WIDTH=getWidth();
	const unsigned HEIGHT=getHeight();
	const float DISTANCE_INF=std::numeric_limits<float>::max();
	const float MIN_WEIGHT=0.5f;

	if(WIDTH<1u || HEIGHT<1u)
		return false;

	// Powiększanie grafu
	if(nodes.size()!=field.size() || (nodes.size()>0u && nodes[0u].size()!=field[0u].size()))
		{
		nodes.resize(field.size());
		for(unsigned y=0u; y<nodes.size(); ++y)
			{
			nodes[y].resize(field[y].size());

			for(unsigned x=0u; x<nodes[y].size(); ++x)
				{
				nodes[y][x].x=x;
				nodes[y][x].y=y;
				nodes[y][x].prev=nullptr;
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

	auto getFieldThreat=[this, getNeighbour, getDistance](GraphNode* node)->float
		{
		float threat=0.0f;

		for(int oy=-1; oy<=1; ++oy)
			{
			for(int ox=-1; ox<=1; ++ox)
				{
				const int x=node->x+ox;
				const int y=node->y+oy;

				const Field* field=getField({x, y});

				if(!field || !field->turret)
					{
					continue;
					}

				switch(field->turret->getType())
					{
					case TurretType::PLAYER_UNIT_SINGLE_TARGET:
						threat+=1.0f;
					break;

					case TurretType::PLAYER_UNIT_AREA_OF_EFFECT:
						threat+=2.5f;
					break;

					case TurretType::PLAYER_UNIT_MINE:
						if(ox==0 && oy==0)
							threat+=9.0f;
					break;

					case TurretType::PLAYER_CARROT_FIELD:
						threat-=1.0f;
					break;

					default:
						//
					break;
					}
				}
			}

		return threat;
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

			const float THREAT=getFieldThreat(neighbour);
			const float WEIGHT=1.0f + THREAT;

			const float alt=node->distance + ((WEIGHT>=MIN_WEIGHT)?WEIGHT:MIN_WEIGHT);

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
		//fprintf(stdout, "\n");
		for(int x=0; x<getWidth(); ++x)
			{
			//if(nodes[y][x].distance==DISTANCE_INF)
			//	fprintf(stdout, "__.__- ");
			//else
			//	fprintf(stdout, "%5.2f%c ", nodes[y][x].distance, (!nodes[y][x].prev)?'-':(nodes[y][x].prev->x<x)?'<':(nodes[y][x].prev->x>x)?'>':(nodes[y][x].prev->y<y)?'^':'v');

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

			if(node->x!=0 || node->y!=0)
				{
				LOG_ERROR("Petla w grafie");
				//return false;
				}
			}
		}
	//fprintf(stdout, "\n");

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
