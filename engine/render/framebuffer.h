/*
 * framebuffer.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

namespace Engine
	{
	namespace Render
		{
		class FrameBuffer
			{
			public:
				enum Buffers
					{
					FBO_NOTHING=       0x0,

					FBO_COLOR_BUFFER=  0x1<<0,
					FBO_DEPTH_BUFFER=  0x1<<1,
					FBO_NORMAL_BUFFER= 0x1<<2,
					FBO_STENCIL_BUFFER=0x1<<3,

					FBO_TEXTURE=       FBO_COLOR_BUFFER,
					FBO_RENDERBUFFER=  FBO_COLOR_BUFFER|FBO_DEPTH_BUFFER,
					};
			private:
				int w, h;

				GLuint fbid; // ID FrameBuffera
				GLuint rbid; // ID RenderBuffera
				GLuint tcid; // ID tekstura (kolor)
				GLuint tnid; // ID tekstura (normale)

				//Camera cam;

			public:
				FrameBuffer(): w(0), h(0), fbid(0u), rbid(0u), tcid(0u), tnid(0u) {}
				~FrameBuffer() {}

				bool init(int w, int h, Buffers flag=FBO_RENDERBUFFER);
				void clear();

				int getWidth() const {return w;}
				int getHeight() const {return h;}
				void getSize(int& gw, int& gh) const {gw=w; gh=h;}
				GLuint getFBO() const {return fbid;}
				GLuint getColorBuffer() const {return tcid;}
				GLuint getNormalBuffer() const {return tnid;}
			};
		} /* namespace Graphics */
	} /* namespace Engine */
