/*
 * bitmapptr.cpp
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#include "bitmapptr.h"

#include "bitmapmanager.h"

using namespace Engine::Graphics;

BitmapPtr::BitmapPtr(const std::string& path): img(nullptr)
	{
	assign(BitmapManager::getInstance().get(path));
	}

void BitmapPtr::assign(Bitmap *nimg)
	{
	if(img)
		{
		img->unref();
		if(img->refs()<1)
			BitmapManager::getInstance().free(img);
		}

	img=nimg;

	if(img)
		img->ref();
	}
