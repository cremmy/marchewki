/*
 * imageptr.h
 *
 *  Created on: 17 kwi 2017
 *      Author: crm
 */

#pragma once

#include <string>

#include "image.h"

namespace Engine
	{
	namespace Graphics
		{
		class ImagePtr
			{
			private:
				const Image *img;

				void assign(const Image *nimg);

			public:
				ImagePtr(): img(nullptr) {}
				ImagePtr(const ImagePtr& iptr): img(nullptr) {assign(iptr.img);}
				ImagePtr(const Image* img): img(nullptr) {assign(img);}
				ImagePtr(const std::string& path);
				~ImagePtr() {assign(nullptr);}

				operator bool() const {return !!img;}
				operator const Image*() const {return img;}

				ImagePtr& operator=(const ImagePtr& iptr) {assign(iptr.img); return *this;}
				//TexturePtr& operator=(TexturePtr&& iptr) {this->img=iptr.img; iptr.img=nullptr; return *this;}
				ImagePtr& operator=(ImagePtr&& iptr) {const Image* tmp=img; img=iptr.img; iptr.img=tmp; return *this;}

				const Image& operator*() const {return *img;}
				const Image* operator->() const {return img;}
			};
		} /* namespace Graphics */
	} /* namespace Engine */
