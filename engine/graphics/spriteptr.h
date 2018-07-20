/*
 * spriteptr.h
 *
 *  Created on: 19 kwi 2017
 *      Author: crm
 */

#pragma once

#include "sprite.h"

#include "../render/shaderptr.h"

namespace Engine
	{
	namespace Graphics
		{
		class SpritePtr
			{
			protected:
				void assign(const Sprite* sprite);

				Render::ShaderPtr shader;

				const Sprite* sprite;
				const Sprite::Animation* animation;
				const Sprite::Direction* direction;
				const Sprite::Frame* frame;

				Sprite::DirectionName iddirection;
				/*unsigned idframe;*/

				float fps;
				float timer;

			public:
				SpritePtr();
				SpritePtr(const SpritePtr& sptr);
				SpritePtr(const Sprite* spr);
				SpritePtr(const std::string& path);
				~SpritePtr();

				operator bool() const {return !!sprite;}
				operator const Sprite*() const {return sprite;}

				SpritePtr& operator=(const SpritePtr& sptr) {assign(sptr.sprite); return *this;}
				//TexturePtr& operator=(TexturePtr&& iptr) {this->img=iptr.img; iptr.img=nullptr; return *this;}
				SpritePtr& operator=(SpritePtr&& sptr)
					{
					const Sprite* sprtmp=sprite;
					sprite=sptr.sprite;
					sptr.sprite=sprtmp;

					shader=sptr.shader;

					animation=sptr.animation;
					direction=sptr.direction;
					frame=sptr.frame;

					iddirection=sptr.iddirection;

					fps=sptr.fps;

					return *this;
					}

				const Sprite& operator*() const {return *sprite;}
				const Sprite* operator->() const {return sprite;}

				void update(float dt);

				void setAnimation(const std::string& name);
				void setAnimation(unsigned idx);
				void setDirection(Sprite::DirectionName direction);
				void setFrame(unsigned idx);
				void setFrameRate(float s) {const unsigned f=getFrameNumber(); fps=s; setFrame(f);}
				void restoreFrameRate(float s) {fps=direction->getFramesPerSecond();}

				Render::ShaderPtr& getShader() {return shader;}
				const Render::ShaderPtr& getShader() const {return shader;}
				Sprite::DirectionName getDirection() const {return iddirection;}
				unsigned getFrameNumber() const {return timer*fps;}
				const Sprite::Frame& getCurrentFrame() const {return *frame;}
			};

		} /* namespace Graphics */
	} /* namespace Engine */
