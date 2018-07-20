/*
 * bitmapptr.h
 *
 *  Created on: 13 kwi 2017
 *      Author: crm
 */

#pragma once

#include "bitmap.h"

namespace Engine
	{
	namespace Graphics
		{
		class BitmapPtr
			{
			private:
				Bitmap* img;

				void assign(Bitmap* nimg);

			public:
				BitmapPtr(): img(nullptr) {}
				BitmapPtr(const BitmapPtr& iptr): img(nullptr) {assign(iptr.img);}
				BitmapPtr(Bitmap* img): img(nullptr) {assign(img);}
				BitmapPtr(const std::string& path);
				~BitmapPtr() {assign(nullptr);}

				operator bool() const {return !!img;}
				operator Bitmap*() {return img;}
				operator const Bitmap*() const {return img;}

				BitmapPtr& operator=(const BitmapPtr& iptr) {assign(iptr.img); return *this;}
				//BitmapPtr& operator=(BitmapPtr&& iptr) {assign(iptr.img); iptr.assign(nullptr); return *this;}
				BitmapPtr& operator=(BitmapPtr&& iptr) {Bitmap* tmp=img; img=iptr.img; iptr.img=tmp; return *this;}

				Bitmap& operator*() {return *img;}
				Bitmap* operator->() {return img;}
				const Bitmap& operator*() const {return *img;}
				const Bitmap* operator->() const {return img;}
			};
		}
	}
