/*
 * imageptr.cpp
 *
 *  Created on: 2 kwi 2017
 *      Author: crm
 */

#include "textureptr.h"

#include "texturemanager.h"

using namespace Engine::Render;

TexturePtr::TexturePtr(const std::string& path): img(nullptr)
	{
	assign(TextureManager::getInstance().get(path));
	}

void TexturePtr::assign(const Texture *nimg)
	{
	if(img)
		{
		img->unref();
		if(img->refs()<1)
			TextureManager::getInstance().free(img);
		}

	img=nimg;

	if(img)
		img->ref();
	}


