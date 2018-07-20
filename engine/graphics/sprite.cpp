/*
 * sprite.cpp
 *
 *  Created on: 19 kwi 2017
 *      Author: crm
 */

#include "sprite.h"

#include "../debug/log.h"

using namespace Engine::Graphics;

constexpr char Sprite::DEFAULT_LAYER[];

std::string Sprite::getDirectionName(DirectionName dir)
	{
	switch(dir)
		{
		case DirectionName::RIGHT:      return "right";
		case DirectionName::RIGHT_DOWN: return "right-down";
		case DirectionName::DOWN:       return "down";
		case DirectionName::LEFT_DOWN:  return "left-down";
		case DirectionName::LEFT:       return "left";
		case DirectionName::LEFT_UP:    return "left-up";
		case DirectionName::UP:         return "up";
		case DirectionName::RIGHT_UP:   return "right-up";

		default:                        return "(none)";
		}
	}

Sprite::DirectionName Sprite::getDirectionByName(const std::string& name)
	{
	if(name==getDirectionName(DirectionName::RIGHT)) return DirectionName::RIGHT;
	if(name==getDirectionName(DirectionName::RIGHT_DOWN)) return DirectionName::RIGHT_DOWN;
	if(name==getDirectionName(DirectionName::DOWN)) return DirectionName::DOWN;
	if(name==getDirectionName(DirectionName::LEFT_DOWN)) return DirectionName::LEFT_DOWN;
	if(name==getDirectionName(DirectionName::LEFT)) return DirectionName::LEFT;
	if(name==getDirectionName(DirectionName::LEFT_UP)) return DirectionName::LEFT_UP;
	if(name==getDirectionName(DirectionName::UP)) return DirectionName::UP;
	if(name==getDirectionName(DirectionName::RIGHT_UP)) return DirectionName::RIGHT_UP;

	return DirectionName::NONE;
	}

Sprite::DirectionName Sprite::getDirectionRotated(Sprite::DirectionName base, float angle)
	{
	int dbase=(int)base;
	dbase=dbase+((angle-22.5f)/90.0f+0.5f)*2.0f;

	while(dbase>=(int)DirectionName::COUNT)
		dbase-=(int)DirectionName::COUNT;
	while(dbase<0)
		dbase+=(int)DirectionName::COUNT;

	return (DirectionName)dbase;
	}


Sprite::Sprite(const std::string& path): RefCounted(), animationDefault(nullptr), path(path)
	{
	//
	}

Sprite::~Sprite()
	{
	//
	}


bool Sprite::finalize()
	{
	LOG_INFO("Sprite.finalize: Wgrywanie klatek do bufora...");

	for(auto& anim: animations)
		{
		for(int didx=0; didx<(int)DirectionName::COUNT; ++didx)
			{
			Direction& dir=anim->getDirection((DirectionName)didx);

			for(unsigned fidx=0u; fidx<dir.getFrameCount(); ++fidx)
				{
				Frame& frame=dir.getFrame(fidx);

				frame.setVBOIndex(vbo.getSize());
				vbo.draw(Math::Orientation::FLAT_XY+frame.getOffset(), frame.getImage());

				//LOG_DEBUG("size: %d", vbo.getSize());
				}
			}
		}

	LOG_INFO("Sprite.finalize: Finalizowanie bufora...");

	if(!vbo.finalize())
		{
		LOG_ERROR("Sprite.finalize: Nie udalo sie wgrac klatek do bufora");
		return false;
		}

	LOG_SUCCESS("Sprite.finalize: Klatki wgrane do bufora");

	return true;
	}

void Sprite::clear()
	{
	for(auto& it: animations)
		{
		delete it;
		}

	animations.clear();
	animationNamed.clear();
	layers.clear();

	shader=nullptr;

	vbo.clear();
	}


Sprite::Animation& Sprite::getAnimation(unsigned idx)
	{
	if(idx>=animations.size())
		return *animations[animations.size()-1u];

	return *animations[idx];
	}

const Sprite::Animation& Sprite::getAnimation(unsigned idx) const
	{
	if(idx>=animations.size())
		return *animations[animations.size()-1u];

	return *animations[idx];
	}

Sprite::Animation& Sprite::getAnimation(const std::string& name)
	{
	auto it=animationNamed.find(name);

	if(it==animationNamed.end())
		return *animationDefault; // Odwazne zalozenie

	return *it->second;
	}

const Sprite::Animation& Sprite::getAnimation(const std::string& name) const
	{
	auto it=animationNamed.find(name);

	if(it==animationNamed.end())
		return *animationDefault;

	return *it->second;
	}

std::string Sprite::getAnimationNameByIDX(unsigned idx) const
	{
	if(idx>=getAnimationCount())
		return "";

	for(auto it: animationNamed)
		{
		if(it.second==animations[idx])
			return it.first;
		}

	return "";
	}


void Sprite::setDefaultAnimation(const std::string& name)
	{
	if(!hasAnimation(name))
		return;

	Animation& animptr=getAnimation(name);

	animationDefault=&animptr;
	}

void Sprite::addLayer(const std::string& name, const Render::TexturePtr& tptr)
	{
	if(!tptr)
		return;

	for(auto& layer: layers)
		{
		if(layer==tptr)
			{
			LOG_WARNING("Sprite.addLayer: Zduplikowana warstwa (\"%s\")", tptr->getName().c_str());
			return;
			}
		}

	/*if(layers.size()<1u)
		layers.push_back(nullptr);*/

	layers.push_back(tptr);

	if(shader)
		{
		shader.uniform(name, tptr);
		}
	}

void Sprite::addAnimation(const std::string& name)
	{
	if(hasAnimation(name))
		return;

	Animation *animptr=new Animation();

	animations.push_back(animptr);
	animationNamed[name]=animptr;

	if(!animationDefault)
		animationDefault=animptr;
	}

void Sprite::addFrame(Direction& dir, const Frame& frame)
	{
	dir.addFrame(frame);

	dir.getFrame(dir.getFrameCount()-1u).setVBOIndex(vbo.getSize());

	vbo.draw(Math::Orientation::FLAT_XY+frame.getOffset(), frame.getImage());
	}

void Sprite::addFrame(const std::string& animname, DirectionName dirname, const Frame& frame)
	{
	Animation* animptr;

	if(!hasAnimation(animname))
		{
		animptr=new Animation();

		animations.push_back(animptr);
		animationNamed[animname]=animptr;
		}
	else
		animptr=&getAnimation(animname);

	if(!animationDefault)
		animationDefault=animptr;

	return addFrame(animptr->getDirection(dirname), frame);
	}


Sprite::Direction& Sprite::Animation::getDirection(DirectionName dir)
	{
	int idir=(int)dir;

	while(idir<0)
		{
		idir+=(int)DirectionName::COUNT;
		}

	//dir=(DirectionName)idir;

	return directions[idir%(int)DirectionName::COUNT];
	/*switch(dir)
		{
		case DirectionName::RIGHT:
		case DirectionName::RIGHT_UP:
		case DirectionName::UP:
		case DirectionName::LEFT_UP:
		case DirectionName::LEFT:
		case DirectionName::LEFT_DOWN:
		case DirectionName::DOWN:
		case DirectionName::RIGHT_DOWN:
			return directions[(int)dir];

		default:
			return directions[(int)DirectionName::DEFAULT];
		}*/
	}

const Sprite::Direction& Sprite::Animation::getDirection(DirectionName dir) const
	{
	while((int)dir<0)
		{
		(int&)dir+=(int)DirectionName::COUNT;
		}

	return directions[(int)dir%(int)DirectionName::COUNT];
	}

Sprite::DirectionName Sprite::Animation::getDirectionClosestExisting(Sprite::DirectionName dir) const
	{
	if(getDirection(dir).getFrameCount()>0u)
		return dir;

	int side=1;

	for(int i=0u; i<(int)DirectionName::COUNT; ++i)
		{
		const DirectionName dn=(DirectionName)(((int)dir+side*(i/2+1))%(int)DirectionName::COUNT);
		const Direction& d=getDirection(dn);

		if(d.getFrameCount()>0u)
			return dn;

		side*=-1;
		}

	return dir;
	}
