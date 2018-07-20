/*
 * image.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "texture.h"

#include <SOIL.h>

using namespace Engine::Render;

bool Texture::dump(const std::string& path) const
	{
	// TODO dump tekstury coś nie chce działać (pusty rysunek)
	const unsigned char *data;

	try
		{
		data=new unsigned char[w*h*4];
		}
	catch(std::bad_alloc&)
		{
		return false;
		}

	GLint boundtid;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundtid);

	glBindTexture(GL_TEXTURE_2D, idtexture);
	glPixelStorei(GL_PACK_ALIGNMENT, 0);
	glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, (void*)data);
	glBindTexture(GL_TEXTURE_2D, boundtid);

	int err=SOIL_save_image(path.c_str(), SOIL_SAVE_TYPE_TGA, w, h, 4, data);

	delete [] data;

	if(err!=0)
		{
		return false;
		}

	return true;
	}
