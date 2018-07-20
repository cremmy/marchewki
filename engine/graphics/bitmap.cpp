/*
 * bitmap.cpp
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#include "bitmap.h"

//#define BITMAP_USE_LIBPNG

#ifndef BITMAP_USE_LIBPNG
#include <SOIL.h>
#else
#include <png.h>
#endif

using namespace Engine::Graphics;

void Bitmap::dump(const std::string& path)
	{
#ifndef BITMAP_USE_LIBPNG
	SOIL_save_image(path.c_str(), SOIL_SAVE_TYPE_TGA, getW(), getH(), 4, (unsigned char*)getPixels());
#else
	FILE *fp= NULL;
	png_structp png_ptr= NULL;
	png_infop info_ptr= NULL;
	png_bytep row= NULL;

	// Open file for writing (binary mode)
	fp=fopen(path.c_str(), "wb");
	if(fp==NULL)
		{
		fprintf(stderr, "Could not open file %s for writing\n", path.c_str());
		goto finalise;
		}

	// Initialize write structure
	png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr==NULL)
		{
		fprintf(stderr, "Could not allocate write struct\n");
		goto finalise;
		}

	// Initialize info structure
	info_ptr=png_create_info_struct(png_ptr);
	if(info_ptr==NULL)
		{
		fprintf(stderr, "Could not allocate info struct\n");
		goto finalise;
		}

	// Setup Exception handling
	if(setjmp(png_jmpbuf(png_ptr)))
		{
		fprintf(stderr, "Error during png creation\n");
		goto finalise;
		}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, this->getW(), this->getH(), 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	/*if(title!=NULL)
		{
		png_text title_text;
		title_text.compression= PNG_TEXT_COMPRESSION_NONE;
		title_text.key="Title";
		title_text.text=title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
		}*/

	png_write_info(png_ptr, info_ptr);

	// Write image data
	int y;
	for(y=0; y<this->getH(); y++)
		{
		png_write_row(png_ptr, (unsigned char*)&this->getPixels()[y*this->getW()]);
		}

	// End write
	png_write_end(png_ptr, NULL);

	finalise: if(fp!=NULL)
		fclose(fp);
	if(info_ptr!=NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if(png_ptr!=NULL)
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if(row!=NULL)
		free(row);

	return;
#endif
	}

void Bitmap::blit(const Bitmap* bmp, int x, int y)
	{
	SDL_Rect r;
	r.x=x;
	r.y=y;

	SDL_UpperBlit(bmp->surface, nullptr, surface, &r);
	}
