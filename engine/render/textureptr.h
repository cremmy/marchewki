/*
 * imageptr.h
 *
 *  Created on: 29 mar 2017
 *      Author: crm
 */

#pragma once

#include "texture.h"

namespace Engine
	{
	namespace Render
		{
		class TexturePtr
			{
			private:
				const Texture *img;

				void assign(const Texture *nimg);

			public:
				TexturePtr(): img(nullptr) {}
				TexturePtr(const TexturePtr& iptr): img(nullptr) {assign(iptr.img);}
				TexturePtr(const Texture* img): img(nullptr) {assign(img);}
				TexturePtr(const std::string& path);
				~TexturePtr() {assign(nullptr);}

				operator bool() const {return !!img;}
				operator const Texture*() const {return img;}
				bool operator==(const TexturePtr& tptr) const {return img==tptr.img;}

				TexturePtr& operator=(const TexturePtr& iptr) {assign(iptr.img); return *this;}
				//TexturePtr& operator=(TexturePtr&& iptr) {this->img=iptr.img; iptr.img=nullptr; return *this;}
				TexturePtr& operator=(TexturePtr&& iptr) {const Texture* tmp=img; img=iptr.img; iptr.img=tmp; return *this;}

				const Texture& operator*() const {return *img;}
				const Texture* operator->() const {return img;}
			};
		}
	}


