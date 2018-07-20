/*
 * atlas.cpp
 *
 *  Created on: 12 kwi 2017
 *      Author: crm
 */

#include "atlas.h"

#include "bitmapmanager.h"
#include "imagemanager.h"

#include "../debug/log.h"
#include "../render/texturemanager.h"

using namespace Engine::Graphics;

Atlas::Atlas(): sizeMin(0), sizeMax(0), size(0), root(nullptr), canvas()
	{
	//
	}

Atlas::~Atlas()
	{
	Atlas::clear();
	}


bool Atlas::init(int sizeMin, int sizeMax)
	{
	LOG_INFO("Atlas.init: Inicjalizacja atlasu...");

	clear();

	this->sizeMin=sizeMin;
	this->sizeMax=sizeMax;
	this->size=sizeMin;

	if(sizeMin<=0 || sizeMax<=0 || sizeMax<sizeMin)
		{
		LOG_ERROR("Atlas.init: Wielkosc atlasu jest za mala (min: %d, max: %d)", sizeMin, sizeMax);
		return false;
		}

	LOG_DEBUG("Atlas.init: Sprawdzanie czy wielkosc jest potega liczby 2 jest wylaczone");
	/*if((sizeMin&(sizeMin-1))!=0 ||(sizeMax&(sizeMax-1))!=0)
		{
		LOG_ERROR("Wielkosc atlasu nie jest potega liczby 2 (min: %d, max: %d)", sizeMin, sizeMax);
		return false;
		}*/

	nodes.reserve(64);

	LOG_SUCCESS("Atlas.init: Udalo sie zainicjalizowac atlas");

	return true;
	}

bool Atlas::addImage(const BitmapPtr& bptr)
	{
	if(!bptr)
		{
		LOG_ERROR("Atlas.addImage: Pusty rysunek");
		LOG_DEBUG("Atlas.addImage: null bptr");
		return false;
		}
	if(bitmapsnamed.find(bptr->getName())!=bitmapsnamed.end())
		{
		LOG_WARNING("Atlas.addImage: Rysunek \"%s\" juz jest w atlasie", bptr->getName().c_str());
		return true;
		}

	bitmaps.push_back(bptr);
	bitmapsnamed[bptr->getName()]=bptr;

	return true;
	}


Atlas::Node* Atlas::Node::insert(const BitmapPtr& bptr)
	{
	Node* ret=nullptr;

	if(child[0])
		{
		ret=child[0]->insert(bptr);

		if(ret)
			return ret;

		return child[1]->insert(bptr);
		}
	else
		{
		if(bitmap)
			return nullptr;

		if(rect.w<bptr->getW() || rect.h<bptr->getH())
			return nullptr;

		if(rect.w==bptr->getW() && rect.h==bptr->getH())
			{
			//LOG_DEBUG("Wpisywanie %d,%d do %d,%d", rect.w, rect.h, rect.x, rect.y);
			bitmap=bptr;
			return this;
			}

		child[0]=new Node();
		child[1]=new Node();

		const int dw=rect.w-bptr->getW();
		const int dh=rect.h-bptr->getH();

		if(dw>dh)
			{
			child[0]->rect=Rectangle(rect.x, rect.y, bptr->getW(), rect.h);
			child[1]->rect=Rectangle(rect.x+bptr->getW(), rect.y, rect.w-bptr->getW(), rect.h);
			}
		else
			{
			child[0]->rect=Rectangle(rect.x, rect.y, rect.w, bptr->getH());
			child[1]->rect=Rectangle(rect.x, rect.y+bptr->getH(), rect.w, rect.h-bptr->getH());
			}

		return child[0]->insert(bptr);
		}
	}


bool Atlas::finalize()
	{
	if(bitmaps.size()<1u)
		{
		if(images.size()>0u)
			{
			LOG_INFO("Atlas.finalize: Atlas zostal juz zamkniety");
			return true;
			}

		LOG_WARNING("Atlas.finalize: Nie podano zadnych rysunkow do atlasu");
		return false;
		}

	if(!doLayout())
		return false;
	if(!doBlit())
		return false;
	if(!doImages())
		return false;

	LOG_SUCCESS("Atlas.finalize: Wygenerowano rysunki z atlasu");

	return true;
	}

bool Atlas::save(const std::string& path)
	{
	if(images.empty())
		{
		if(canvas)
			{
			canvas->dump(path);
			return true;
			}

		return false;
		}

	return images[0]->getTexture()->dump(path);
	}


bool Atlas::doLayout()
	{
	if(!images.empty())
		{
		LOG_ERROR("Atlas.doLayout: Atlas zostal zamkniety");
		return false;
		}
	if(bitmaps.empty())
		{
		LOG_ERROR("Atlas.doLayout: Brak rysunkow w atlasie");
		return false;
		}
	if(root)
		{
		LOG_WARNING("Atlas.doLayout: Ponowne rozkladanie rysunkow");
		delete root;
		}

	LOG_INFO("Atlas.doLayout: Ukladanie rysunkow w atlasie");
	LOG_DEBUG("Atlas.doLayout: [bitmaps %u][min %d][max %d]", bitmaps.size(), sizeMin, sizeMax);

	bool success=false;

	root=nullptr;

	while(!success)
		{
		root=new Node();
		root->rect=Rectangle(0, 0, size, size);
		nodes.clear();

		success=true;

		for(const BitmapPtr& bptr: bitmaps)
			{
			Node* node=root->insert(bptr);

			if(!node)
				{
				LOG_WARNING("Atlas.doLayout: Atlas za maly, powiekszanie z %d do %d (max %d)", size, 2*size, sizeMax);

				size*=2;

				delete root;
				success=false;

				break;
				}

			nodes.push_back(node);
			}

		if(!success && size>sizeMax)
			{
			LOG_ERROR("Atlas.doLayout: Przekroczono maksymalna wielkosc atlasu (jest %d, max %d)", size, sizeMax);
			break;
			}
		}

	if(!success)
		{
		LOG_ERROR("Atlas.doLayout: Nie udalo sie ulozyc rysunkow w atlasie");
		return false;
		}

	LOG_SUCCESS("Atlas.doLayout: Udalo sie rozlozyc rysunki w atlasie");
	LOG_DEBUG("Atlas.doLayout: Wielkosc %d, %dpx", size, size);

	return true;
	}

bool Atlas::doBlit()
	{
	if(!images.empty())
		{
		LOG_ERROR("Atlas.doBlit: Atlas zostal zamkniety");
		return false;
		}
	if(bitmaps.empty())
		{
		LOG_ERROR("Atlas.doBlit: Brak rysunkow w atlasie");
		return false;
		}
	if(!root)
		{
		LOG_WARNING("Atlas.doBlit: Rysunki nie zostaly rozlozone, wywolywanie doLayout");

		if(!doLayout())
			return false;
		}
	if(canvas)
		{
		LOG_WARNING("Atlas.doBlit: Ponowne wpisywanie rysunkow");
		canvas=nullptr;
		}

	LOG_INFO("Atlas.doBlit: Wpisywanie rysunkow do atlasu...");

	canvas=BitmapManager::getInstance().get(size, size);

	for(auto node: nodes)
		{
		//LOG_DEBUG("[%p    %d %d  %d %d][%p %p]", cnode, cnode->rect.x, cnode->rect.y, cnode->rect.w, cnode->rect.h, cnode->child[0], cnode->child[1]);

		if(node->bitmap)
			{
			//LOG_DEBUG("Blit %p at %d %d", cnode, cnode->rect.x, cnode->rect.y);
			canvas->blit((Bitmap*)node->bitmap, node->rect.x, node->rect.y);
			}
		}

	//canvas->dump("atlas.bmp");

	LOG_SUCCESS("Atlas.doBlit: Rysunki wpisane do atlasu");

	return true;
	}

bool Atlas::doImages()
	{
	if(!images.empty())
		{
		LOG_ERROR("Atlas.doImages: Atlas zostal zamkniety");
		return false;
		}
	if(bitmaps.empty())
		{
		LOG_ERROR("Atlas.doImages: Brak rysunkow w atlasie");
		return false;
		}
	if(!root)
		{
		LOG_WARNING("Atlas.doImages: Rysunki nie zostaly rozlozone, rozkladanie...");

		if(!doLayout())
			return false;
		}
	if(!canvas)
		{
		LOG_WARNING("Atlas.doImages: Rysunki nie zostaly wpisane, wpisywanie...");

		if(!doBlit())
			return false;
		}

	LOG_INFO("Atlas.doImages: Generowanie tekstury...");

	texture=Render::TextureManager::getInstance().get(*canvas);

	if(!texture)
		{
		LOG_ERROR("Atlas.doImages: Nie udalo sie wygenerowac tekstury");
		return false;
		}

	LOG_SUCCESS("Atlas.doImages: Tekstura wygenerowana");
	LOG_DEBUG("Atlas.doImages: Kasowanie listy bitmap i zwalnianie canvasa");

	bitmaps.clear();
	bitmapsnamed.clear();
	canvas=nullptr;

	//Wrzucanie ImagePtrow do ImageManagera
	LOG_INFO("Atlas.doImages: Generowanie rysunkow...");

	for(auto node: nodes)
		{
		if(node->bitmap)
			{
			ImagePtr iptr=ImageManager::getInstance().get(texture, node->rect.x, node->rect.y, node->rect.w, node->rect.h, node->bitmap->getName());

			if(!iptr)
				{
				LOG_ERROR("Atlas.doImages: Nie udalo sie utowrzyc rysunku \"%s\" z tekstury", node->bitmap->getName().c_str());
				return false;
				}

			images.push_back(iptr);
			}
		}

	LOG_DEBUG("Atlas.doImages: Kasowanie drzewa...");
	delete root;
	root=nullptr;
	LOG_DEBUG("Atlas.doImages: Drzewo usuniete");

	return true;
	}


bool Atlas::getImageInfo(const std::string& path, int& x, int& y, int& w, int& h)
	{
	if(imagesnamed.find(path)!=imagesnamed.end())
		{
		ImagePtr& iptr=imagesnamed[path];

		x=iptr->getX();
		y=iptr->getY();
		w=iptr->getW();
		h=iptr->getH();

		return true;
		}

	if(root)
		{
		std::vector<Node*> nodes;

		nodes.push_back(root);
		while(!nodes.empty())
			{
			Node* cnode=nodes.back();
			nodes.pop_back();

			if(cnode->bitmap && cnode->bitmap->getName()==path)
				{
				x=cnode->rect.x;
				y=cnode->rect.y;
				w=cnode->rect.w;
				h=cnode->rect.h;

				return true;
				}

			if(cnode->child[0])
				{
				nodes.push_back(cnode->child[0]);
				nodes.push_back(cnode->child[1]);
				}
			}
		}

	return false;
	}


ImagePtr Atlas::getImage(unsigned i) const
	{
	if(i>=images.size())
		return ImagePtr(nullptr);

	return images[i];
	}

ImagePtr Atlas::getImage(const std::string& path) const
	{
	auto it=imagesnamed.find(path);

	if(it==imagesnamed.end())
		return ImagePtr(nullptr);

	return it->second;
	}


void Atlas::clear()
	{
	bitmaps.clear();
	bitmapsnamed.clear();
	images.clear();
	imagesnamed.clear();
	texture=nullptr;
	delete root;
	root=nullptr;
	nodes.clear();
	canvas=nullptr;
	}
