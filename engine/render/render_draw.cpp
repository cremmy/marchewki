/*
 * render_draw.cpp
 *
 *  Created on: 14 cze 2017
 *      Author: crm
 */

#include "render.h"

#include "model.h"
#include "texture.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include "../consts.h"
#include "../debug/assert.h"
#include "../graphics/imageptr.h"
#include "../graphics/spriteptr.h"

using namespace Engine::Render;
using namespace Engine::Math;

void Render::drawLine(const Vector& a, const Vector& b, const Math::Vector& color)
	{
	statePush();

	Vertex data[]={
		Vertex(a.x, a.y, a.z,   0.0f, 0.0f),
		Vertex(b.x, b.y, b.z,   1.0f, 1.0f)
		};

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	setShader(baseShaderPrimitive);

	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &color.x);

	glLineWidth(4.0f);
	glDisable(GL_TEXTURE_2D);
	glDrawArrays(GL_LINES, 0, 2);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(1.0f);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	statePop();
	}

void Render::drawPolygon(const std::vector<Vector>& vertices, const Vector& color, const Vector& fill)
	{
	statePush();

	std::vector<Vertex> data;
	data.reserve(vertices.size()+1u);

	Vector center=Vector(0, 0, 0);
	for(auto vertex: vertices)
		{
		center+=vertex/(float)vertices.size();
		}

	data.push_back({center.x, center.y, center.z, 0.0f, 0.0f});
	for(auto vertex: vertices)
		{
		data.push_back(Vertex(vertex.x, vertex.y, vertex.z, 0.0f, 0.0f));
		}
	data.push_back({vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f});

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*data.size(), data.data(), GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	setShader(baseShaderPrimitive);

	glDisable(GL_TEXTURE_2D);
	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &color.x);
	//glLineWidth(2.0f);
	glDrawArrays(GL_LINE_STRIP, 1, data.size()-1u);
	//glLineWidth(1.0f);
	glUniform4fv(baseShaderPrimitive->getUniform(SHADER_UNIFORM_COLOR), 1, &fill.x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, data.size());
	glEnable(GL_TEXTURE_2D);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	statePop();
	}


void Render::draw(const Orientation& orientation, const VertexBuffer& vbo)
	{
	State& state=states.back();

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());

	vbo.bind();

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	glDrawArrays(GL_QUADS, 0, vbo.getSize());

	vbo.unbind();
	}

void Render::draw(const Orientation& orientation, const VertexBuffer& vbo, unsigned first, unsigned last)
	{
	assert(first<last);
	assert(last<vbo.getSize());

	State& state=states.back();

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());

	vbo.bind();

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	glDrawArrays(GL_QUADS, first, last-first);

	vbo.unbind();
	}

void Render::draw(const Math::Orientation& orientation, const FrameBuffer& fbo)
	{
	//assert(fbo.getColorBuffer());

	State& state=states.back();

	const float iw=fbo.getWidth();
	const float ih=fbo.getHeight();

	Vertex data[]=
		{
		Vertex( 0,  0, 0,  0, 0),
		Vertex(iw,  0, 0,  1, 0),
		Vertex(iw,-ih, 0,  1, 1),
		Vertex( 0,-ih, 0,  0, 1),
		};

	bind(0u, fbo.getColorBuffer());

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}

void Render::draw(const Orientation& orientation, const Model& mdl)
	{
	statePush();
	State& state=states.back();

	if(!!mdl.getShader())
		setShader(mdl.getShader());

	const VertexBuffer& vbo=mdl.getVBO();

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());

	vbo.bind();

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	GLuint ubidx=state.shader->getUniformBlock("Material");
	if(ubidx!=GL_INVALID_INDEX)
		{
		glUniformBlockBinding(state.shader->getProgramID(), ubidx, 2u); // 2u -> jak wyżej
		glBindBufferBase(GL_UNIFORM_BUFFER, 2u, mdl.getUBO()); // 2u -> indeks na którym bindowane jest UBO
		}

	glDrawArrays(GL_TRIANGLES, 0, vbo.getSize());

	vbo.unbind();
	statePop();
	}


void Render::draw(const Orientation& orientation, const Texture& tex)
	{
	assert(tex.getTextureID());

	draw(orientation, tex, 0, 0, tex.getW(), tex.getH());
	}

void Render::draw(const Orientation& orientation, const Texture& tex, float x, float y, float w, float h)
	{
	assert(tex.getTextureID());

	State& state=states.back();

	const float iw=tex.getW();
	const float ih=tex.getH();

	Vertex data[]=
		{
		Vertex(0, 0, 0,  x   /iw,  y   /ih),
		Vertex(w, 0, 0, (x+w)/iw,  y   /ih),
		Vertex(w,-h, 0, (x+w)/iw, (y+h)/ih),
		Vertex(0,-h, 0,  x   /iw, (y+h)/ih),
		};

	bind(0u, tex.getTextureID());

	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}

void Render::draw(const Orientation& orientation, const TexturePtr& tptr)
	{
	assert(!!tptr);

	draw(orientation, *tptr, 0, 0, tptr->getW(), tptr->getH());
	}

void Render::draw(const Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h)
	{
	assert(!!tptr);

	draw(orientation, *tptr, x, y, w, h);
	}

void Render::draw(const Orientation& orientation, const Graphics::ImagePtr& iptr)
	{
	assert(!!iptr);

	draw(orientation, *iptr->getTexture(), iptr->getX(), iptr->getY(), iptr->getW(), iptr->getH());
	}

void Render::draw(const Orientation& orientation, const Graphics::SpritePtr& sptr)
	{
	assert(!!sptr);

	//drawLine(orientation.getPosition(), orientation.getPosition()+orientation.getRight()*32, Vector(1, 0, 0, 1));
	//drawLine(orientation.getPosition(), orientation.getPosition()+orientation.getForward()*32, Vector(0, 1, 0, 1));
	//drawLine(orientation.getPosition(), orientation.getPosition()+orientation.getUp()*32, Vector(0, 0, 1, 1));

	State& state=states.back();

	if(sptr.getShader())
		{
		setShader(sptr.getShader());
		}
	else
		{
		setShader(baseShaderImage);
		}

	const Graphics::Sprite::Frame& frame=sptr.getCurrentFrame();

	bind(frame.getImage()->getTexture());

	//draw(orientation/*-frame.getOffset()*/, frame.getImage());

	glBindBuffer(GL_ARRAY_BUFFER, sptr->getVertexBuffer().getVBO());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));

	const Matrix mmodel=MatrixTranspose(orientation.getMatrix());
	glUniformMatrix4fv(state.shader->getUniform(SHADER_UNIFORM_MODEL_MATRIX), 1, GL_TRUE, &mmodel.row[0].x);
	glUniform4fv(state.shader->getUniform(SHADER_UNIFORM_COLOR), 1, &state.color.x);

	glDrawArrays(GL_QUADS, frame.getVBOIndex(), 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	}
