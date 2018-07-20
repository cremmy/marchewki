/*
 * sprite.h
 *
 *  Created on: 19 kwi 2017
 *      Author: crm
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "imageptr.h"

#include "../core/refcounted.h"
#include "../math/math.h"
#include "../render/shaderptr.h"
#include "../render/vertexbuffer.h"

namespace Engine
	{
	namespace Graphics
		{
		class SpriteManager;

		class Sprite: public Engine::Core::RefCounted
			{
			public:
				static constexpr const char DEFAULT_LAYER[]="default";

				enum class DirectionName: unsigned
					{
					RIGHT=0,
					RIGHT_UP=1,
					UP=2,
					LEFT_UP=3,
					LEFT=4,
					LEFT_DOWN=5,
					DOWN=6,
					RIGHT_DOWN=7,

					COUNT=8,

					DEFAULT=DOWN,
					NONE,
					};

				static std::string getDirectionName(DirectionName dir);
				static DirectionName getDirectionByName(const std::string& name);
				static DirectionName getDirectionRotated(DirectionName base, float angle);

				class Frame
					{
					protected:
						ImagePtr image;
						unsigned vboidx;
						int ox, oy;

						std::map<std::string, Engine::Math::Vector> miscPoints;

						inline void setVBOIndex(unsigned s) {vboidx=s;}

						friend Sprite;

					public:
						Frame(): image(nullptr), vboidx(0u), ox(0), oy(0) {}

						inline ImagePtr& getImage() {return image;}
						inline const ImagePtr& getImage() const {return image;}
						inline int getWidth() const {return image->getW();}
						inline int getHeight() const {return image->getH();}
						inline unsigned getVBOIndex() const {return vboidx;}
						Engine::Math::Vector getPoint(const std::string& name) const
							{
							auto it=miscPoints.find(name);
							if(it==miscPoints.end())
								return Engine::Math::Vector();

							return it->second;
							}
						inline Engine::Math::Vector getOffset() const {return Engine::Math::Vector(ox, oy);}

						void addPoint(const std::string& name, const Engine::Math::Vector& pos) {miscPoints[name]=pos;}

						inline void setImage(const ImagePtr& s) {if(s) image=s;}
						inline void setOffset(int sx, int sy) {ox=-sx; oy=sy;}
					};

				class Direction
					{
					protected:
						std::vector<Frame> frames;

						float fps;       // Liczba klatek na sekunde
						unsigned loopto; // Numer klatki do której animacja powinna się zapętlić

					public:
						Direction(): fps(0.0f), loopto(0u) {}
						~Direction() {frames.clear();}

						inline Frame& operator[](unsigned idx) {return getFrame(idx);}
						inline const Frame& operator[](unsigned idx) const {return getFrame(idx);}

						inline float getFramesPerSecond() const {return fps;}
						inline float getAnimationTime() const {return frames.size()/fps;}
						inline unsigned getLoopFrameIndex() const {return loopto;}
						inline unsigned getFrameCount() const {return frames.size();}
						Frame& getFrame(unsigned idx)
							{
							// Miejmy nadzieje, ze liczba klatek zawsze bedzie wieksza od 1

							if(idx>=getFrameCount())
								return frames[getFrameCount()-1u];
							return frames[idx];
							}
						const Frame& getFrame(unsigned idx) const
							{
							if(idx>=getFrameCount())
								return frames[getFrameCount()-1u];
							return frames[idx];
							}

						inline void setFramesPerSecond(float s) {fps=s;}
						inline void setLoopFrameIndex(int s) {loopto=(s<0 || s>=(int)getFrameCount())?getFrameCount()-1u:s;}

						void addFrame(const Frame& frame) {frames.push_back(frame);}
					};

				class Animation
					{
					protected:
						Direction directions[(unsigned)DirectionName::COUNT];

					public:
						Animation()	{}
						~Animation() {}

						inline Direction& operator[](DirectionName dir) {return getDirection(dir);}
						inline const Direction& operator[](DirectionName dir) const {return getDirection(dir);}

						Direction& getDirection(DirectionName dir);
						const Direction& getDirection(DirectionName dir) const;
						DirectionName getDirectionClosestExisting(DirectionName dir) const;
					};

			protected:
				std::vector<Animation*> animations;
				std::map<std::string, Animation*> animationNamed;
				std::vector<Render::TexturePtr> layers;

				Animation* animationDefault;

				Render::ShaderPtr shader;
				Render::VertexBuffer vbo;

				std::string path;
				std::string name;

			public:
				Sprite(const std::string& path="");
				~Sprite();

				inline Animation& operator[](unsigned idx) {return getAnimation(idx);}
				inline const Animation& operator[](unsigned idx) const {return getAnimation(idx);}
				inline Animation& operator[](const std::string& name) {return getAnimation(name);}
				inline const Animation& operator[](const std::string& name) const {return getAnimation(name);}

				bool finalize();
				void clear();

				inline unsigned getAnimationCount() const {return animations.size();}
				inline bool hasAnimation(const std::string& name) {return animationNamed.find(name)!=animationNamed.end();}
				inline const Animation& getDefaultAnimation() const {return *animationDefault;}
				inline unsigned getLayerCount() const {return layers.size();}
				inline const Render::TexturePtr& getLayer(unsigned i) const {return layers[i];}
				inline const Render::ShaderPtr& getShader() const {return shader;}
				inline const Render::VertexBuffer& getVertexBuffer() const {return vbo;}
				inline std::string getPath() const {return path;}
				inline std::string getName() const {return name;}
				Animation& getAnimation(unsigned idx);
				const Animation& getAnimation(unsigned idx) const;
				Animation& getAnimation(const std::string& name);
				const Animation& getAnimation(const std::string& name) const;
				std::string getAnimationNameByIDX(unsigned idx) const;

				void addLayer(const std::string& name, const Render::TexturePtr& tptr);
				void addAnimation(const std::string& name);
				void addFrame(Direction& dir, const Frame& frame);
				void addFrame(const std::string& animname, DirectionName dirname, const Frame& frame);

				void setShader(const Render::ShaderPtr& s) {shader=s;}
				void setDefaultAnimation(const std::string& name);
				//void setPath(const std::string& s) {path=s;}
				void setName(const std::string& s) {name=s;}

				friend class SpriteManager;
			};

		inline Sprite::DirectionName operator+(Sprite::DirectionName base, float angle) {return Sprite::getDirectionRotated(base, angle);}
		} /* namespace Graphics */
	} /* namespace Engine */
