/*
 * framebuffer.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "framebuffer.h"

#include "../debug/log.h"

using namespace Engine::Render;

bool FrameBuffer::init(int w, int h, Buffers flag)
	{
	clear();

	LOG_DEBUG("FrameBuffer.init: Tworzenie FrameBuffera, %dx%dpx", w, h);

	GLuint err;

	// Renderbuffer
	if(!(flag&FBO_DEPTH_BUFFER) && !(flag&FBO_STENCIL_BUFFER))
		{
		LOG_DEBUG("FrameBuffer.init: DepthBuffer nie zostanie utworzony");
		}
	else
		{
		GLint boundrbid=0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &boundrbid);

		LOG_DEBUG("Aktualnie zbindowany render buffer: %d", boundrbid);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: %d", __LINE__);
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			//return false;
			}

		glGenRenderbuffers(1, &rbid);

		if(!rbid)
			{
			LOG_ERROR("FrameBuffer.init: Nie udalo sie utworzyc RenderBuffer");
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(glGetError()));
			return false;
			}

		glBindRenderbuffer(GL_RENDERBUFFER, rbid);

		if(flag&FBO_STENCIL_BUFFER)
			{
			LOG_DEBUG("FrameBuffer.init: GL_DEPTH24_STENCIL8");
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
			}
		else
			{
			LOG_DEBUG("FrameBuffer.init: GL_DEPTH_COMPONENT");
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h); // GL_DEPTH_COMPONENT powoduje automatyczny wybor precyzji depth buffera
			}

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: Nie udalo sie zajac pamieci na RenderBuffer");
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			return false;
			}
		else
			{
			LOG_DEBUG("FrameBuffer.init: RenderBuffer zrobiony");
			}

		if(boundrbid)
			glBindRenderbuffer(GL_RENDERBUFFER, boundrbid);
		}

	// Tekstura
	if(!(flag&FBO_COLOR_BUFFER))
		{
		LOG_DEBUG("FrameBuffer.init: ColorBuffer nie zostanie utworzony");
		}
	else
		{
		GLint boundtid;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundtid);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: %d", __LINE__);
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			//return false;
			}

		glGenTextures(1, &tcid);

		if(!tcid)
			{
			LOG_ERROR("FrameBuffer.init: Nie udalo sie utworzyc tekstury");
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(glGetError()));
			return false;
			}

		glBindTexture(GL_TEXTURE_2D, tcid);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glBindTexture(GL_TEXTURE_2D, boundtid);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: Nie udalo sie zajac pamieci na teksture");
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			return false;
			}
		else
			{
			LOG_DEBUG("FrameBuffer.init: Tekstura zrobiona");
			}
		}

	// Framebuffer
	GLint boundfbid;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundfbid);

	if((err=glGetError())!=GL_NO_ERROR)
		{
		LOG_ERROR("FrameBuffer.init: %d", __LINE__);
		LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
		//return false;
		}

	glGenFramebuffers(1, &fbid);

	if(!fbid)
		{
		LOG_ERROR("FrameBuffer.init: Nie udalo sie utworzyc FrameBuffer");
		LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(glGetError()));
		return false;
		}

	LOG_DEBUG("Bindowanie FBO: %d", fbid);

	glBindFramebuffer(GL_FRAMEBUFFER, fbid);

	if((err=glGetError())!=GL_NO_ERROR)
		{
		LOG_ERROR("FrameBuffer.init: %d", __LINE__);
		LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
		//return false;
		}

	if(flag&FBO_COLOR_BUFFER)
		{
		LOG_DEBUG("FrameBuffer.init: Bind color, tex %d", tcid);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tcid, 0);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: %d", __LINE__);
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			//return false;
			}
		}

	if(flag&FBO_STENCIL_BUFFER)
		{
		LOG_DEBUG("FrameBuffer.init: Bind depth/stencil");
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbid);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: %d", __LINE__);
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			//return false;
			}
		}
	else if(flag&FBO_DEPTH_BUFFER)
		{
		LOG_DEBUG("FrameBuffer.init: Bind depth");
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbid);

		if((err=glGetError())!=GL_NO_ERROR)
			{
			LOG_ERROR("FrameBuffer.init: %d", __LINE__);
			LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
			//return false;
			}
		}

	if(GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
		{
		LOG_ERROR("FrameBuffer.init: FrameBuffer nie jest kompletny");
		LOG_ERROR("FrameBuffer.init: Blad: %d", status);

		switch(status)
			{
			case GL_FRAMEBUFFER_UNDEFINED: LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_UNDEFINED "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER "); break;
			case GL_FRAMEBUFFER_UNSUPPORTED  : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_UNSUPPORTED  "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE  : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE  "); break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS   : LOG_ERROR("FrameBuffer.init: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS "); break;
			case GL_INVALID_ENUM    : LOG_ERROR("FrameBuffer.init: GL_INVALID_ENUM "); break;
			}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, boundfbid);

		return false;
		}
	else
		{
		LOG_SUCCESS("FrameBuffer.init: Utworzono FrameBuffer [GLid: %u]", fbid);
		}

	//glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, boundfbid);

	if((err=glGetError())!=GL_NO_ERROR)
		{
		LOG_ERROR("FrameBuffer.init: Nie udalo sie zbindowac poprzedniego FBO (id: %u)", boundfbid);
		LOG_ERROR("FrameBuffer.init: Blad: %s", gluErrorString(err));
		return false;
		}

	this->w=w;
	this->h=h;

	//cam=Camera(Camera::ORTHOGRAPHIC, w, h, 1, 10000);

	return true;
	}

void FrameBuffer::clear()
	{
	if(!fbid)
		return;

	if(rbid)
		glDeleteRenderbuffers(1, &rbid);

	if(tcid)
		glDeleteTextures(1, &tcid);

	if(tnid)
		glDeleteTextures(1, &tnid);

	if(fbid)
		glDeleteFramebuffers(1, &fbid);

	LOG_DEBUG("FrameBuffer.clear: Kasowanie FBO [GLid: %u]", fbid);

	rbid=0u;
	tcid=0u;
	tnid=0u;
	fbid=0u;

	w=0;
	h=0;
	}

