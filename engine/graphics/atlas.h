/*
 * atlas.h
 *
 *  Created on: 12 kwi 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "bitmapptr.h"
#include "imageptr.h"
#include "../render/textureptr.h"

namespace Engine
	{
	namespace Graphics
		{
		class Atlas
			{
			protected:
				struct Rectangle
					{
					Rectangle(int x, int y, int w, int h): x(x), y(y), w(w), h(h) {}

					bool overlap(const Rectangle& r)
						{
						if(x+w<r.x || x>r.x+r.w || y+h<r.y || y>r.y+r.h)
							return false;
						return true;
						}

					bool isInside(const Rectangle& r)
						{
						if(x<r.x || x+w>r.x+r.w || y<r.y || y+h>r.y+r.h)
							return false;
						return true;
						}

					int x, y, w, h;
					};

				struct Node
					{
					Node(): rect(0, 0, 0, 0)
						{
						child[0]=nullptr;
						child[1]=nullptr;
						}
					~Node()
						{
						if(child[0])
							delete child[0];
						if(child[1])
							delete child[1];
						}

					Node* insert(const BitmapPtr& bptr);

					Node *child[2];
					Rectangle rect;
					BitmapPtr bitmap;
					};

				int sizeMin;
				int sizeMax;
				int size;

				std::vector<BitmapPtr> bitmaps;
				std::map<std::string, BitmapPtr> bitmapsnamed;
				Render::TexturePtr texture;
				std::vector<ImagePtr> images;
				std::map<std::string, ImagePtr> imagesnamed;

				Node* root;
				std::vector<Node*> nodes;
				BitmapPtr canvas;

			public:
				Atlas();
				~Atlas();

				bool init(int sizeMin=128, int sizeMax=4096);
				bool addImage(const BitmapPtr& bptr);
				// Wywoluje doLayout, doBlit oraz doImages
				bool finalize();
				bool save(const std::string& path);

				// Ponizsze funkcje powinny byc wywolywane po kolei
				bool doLayout();
				bool doBlit();
				bool doImages();

				unsigned getImageCount() const {return images.empty()?bitmaps.size():images.size();}
				bool getImageInfo(const std::string& path, int& x, int& y, int& w, int& h);

				Render::TexturePtr getTexture() const {return texture;}
				ImagePtr getImage(unsigned i) const;
				ImagePtr getImage(const std::string& path) const;

				void clear();
			};

		} /* namespace Graphics */
	} /* namespace Engine */
