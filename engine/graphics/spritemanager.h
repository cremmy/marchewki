/*
 * spritemanager.h
 *
 *  Created on: 31 maj 2017
 *      Author: crm
 */

#pragma once

#include "spriteptr.h"

namespace Engine
	{
	namespace Graphics
		{
		class SpriteManager
			{
			protected:
				SpriteManager();
				SpriteManager(const SpriteManager& )=delete;
				~SpriteManager();
				SpriteManager& operator=(const SpriteManager& )=delete;

				std::vector<Sprite*> sprites;
				std::map<std::string, Sprite*> named;

				bool isValid(Sprite& spr);
				//bool getFromImage(Sprite& spr, const std::string& path);
				bool getFromXML(Sprite& spr, const std::string& path);

			public:
				static SpriteManager& getInstance()
					{
					static SpriteManager instance;
					return instance;
					}

				SpritePtr get(const std::string& path);

				void free(const Sprite* spr, bool erase=true);

				/*void clear();*/
			};

		} /* namespace Graphics */
	} /* namespace Engine */
