/*
 * imageptr.cpp
 *
 *  Created on: 17 kwi 2017
 *      Author: crm
 */

#include "imageptr.h"

#include "image.h"
#include "imagemanager.h"

using namespace Engine::Graphics;

ImagePtr::ImagePtr(const std::string& path): img(nullptr)
	{
	assign(ImageManager::getInstance().get(path));
	}

void ImagePtr::assign(const Image *nimg)
	{
	if(img)
		{
		img->unref();
		if(img->refs()<1)
			ImageManager::getInstance().free(img);
		}

	img=nimg;

	if(img)
		img->ref();
	}
