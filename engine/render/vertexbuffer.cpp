/*
 * vertexbuffer.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "vertexbuffer.h"

#include <fstream>

#include "textureptr.h"
#include "../debug/log.h"
#include "../graphics/spriteptr.h"


using namespace Engine::Render;

bool VertexBuffer::init(Vertex *vertices, unsigned size)
	{
	GLenum err;
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("VertexBuffer.init: Inicjalizacja bufora, %u wierzcholkow", size);

	if(!vboid)
		{
		LOG_DEBUG("VertexBuffer.init: Tworzenie VBO");

		glGenBuffers(1, &vboid);

		err=glGetError();
		if(err!=GL_NO_ERROR)
			{
			LOG_ERROR("VertexBuffer.init: Nie udalo sie utworzyc VBO");
			LOG_ERROR("VertexBuffer.init: %s", gluErrorString(err));
			return false;
			}

		LOG_SUCCESS("VertexBuffer.init: Utworzono VertexBuffer [GLid: %u]", vboid);
		}

	/*glGenVertexArrays(1, &vaoid);
	glBindVertexArray(vaoid);*/

	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	LOG_DEBUG("VertexBuffer.init: Wgrywanie danych do VBO [GLid: %u]", vboid);

	this->size=size;
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("VertexBuffer.init: Nie udalo sie wgrac danych do VBO [GLid: %u]", vboid);
		LOG_ERROR("VertexBuffer.init: %s", gluErrorString(err));
		return false;
		}

	LOG_SUCCESS("VertexBuffer.init: Wgranych wierzcholkow: %u [GLid: %u]", size, vboid);

	return true;
	}

bool VertexBuffer::finalize()
	{
	if(isFinalized())
		return true;

	if(vertices.size()<1u)
		{
		LOG_ERROR("VertexBuffer.finalize: Nie wgrano wierzcholkow");
		return false;
		}

	GLenum err;
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("VertexBuffer.finalize: Finalizowanie bufora, %u wierzcholkow", vertices.size());

	if(!vboid)
		{
		LOG_DEBUG("VertexBuffer.finalize: Tworzenie VBO");

		glGenBuffers(1, &vboid);

		err=glGetError();
		if(err!=GL_NO_ERROR)
			{
			LOG_ERROR("VertexBuffer.finalize: Nie udalo sie utworzyc VBO");
			LOG_ERROR("VertexBuffer.finalize: %s", gluErrorString(err));
			return false;
			}

		LOG_SUCCESS("VertexBuffer.finalize: Utworzono VertexBuffer [GLid: %u]", vboid);
		}

	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	LOG_DEBUG("VertexBuffer.finalize: Wgrywanie danych do VBO [GLid: %u]", vboid);

	this->size=vertices.size();
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	err=glGetError();
	if(err!=GL_NO_ERROR)
		{
		LOG_ERROR("VertexBuffer.finalize: Nie udalo sie wgrac danych do VBO [GLid: %u]", vboid);
		LOG_ERROR("VertexBuffer.finalize: %s", gluErrorString(err));
		return false;
		}

	vertices.clear();

	LOG_SUCCESS("VertexBuffer.finalize: Wgranych wierzcholkow: %u [GLid: %u]", size, vboid);

	return true;
	}

void VertexBuffer::flush()
	{
	size=0u;
	}

void VertexBuffer::clear()
	{
	LOG_INFO("VertexBuffer.finalize: Czyszczenie bufora [GLid: %u][size: %u]", vboid, vertices.size());

	if(vboid)
		{
		glDeleteBuffers(1, &vboid);
		}

	vboid=0u;

	vertices.clear();
	}


void VertexBuffer::bind() const
	{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tx));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ntx));
	}

void VertexBuffer::unbind() const
	{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	}


bool VertexBuffer::add(const Vertex& vertex)
	{
	vertices.push_back(vertex);

	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const TexturePtr& tptr)
	{
	using namespace Engine::Math;

	const Vector p=orientation.getPosition();
	const Vector r=orientation.getRight();
	const Vector u=orientation.getUp();
	//const Vector f=orientation.getForward();

	const Vector v1=p;
	const Vector v2=p+r*tptr->getW();
	const Vector v3=p+r*tptr->getW()-u*tptr->getH();
	const Vector v4=p               -u*tptr->getH();

	if(!add(Vertex(v1.x, v1.y, v1.z, 0, 0)) ||
	   !add(Vertex(v2.x, v2.y, v2.z, 1, 0)) ||
	   !add(Vertex(v3.x, v3.y, v3.z, 1, 1)) ||
	   !add(Vertex(v4.x, v4.y, v4.z, 0, 1)))
		return false;
	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h)
	{
	using namespace Engine::Math;

	const Vector p=orientation.getPosition();
	const Vector r=orientation.getRight();
	const Vector u=orientation.getUp();
	//const Vector f=orientation.getForward();

	const Vector v1=p;
	const Vector v2=p+r*w;
	const Vector v3=p+r*w-u*h;
	const Vector v4=p    -u*h;

	const float iw=tptr->getW();
	const float ih=tptr->getH();

	if(!add(Vertex(v1.x, v1.y, v1.z,  x   /iw,  y   /ih)) ||
	   !add(Vertex(v2.x, v2.y, v2.z, (x+w)/iw,  y   /ih)) ||
	   !add(Vertex(v3.x, v3.y, v3.z, (x+w)/iw, (y+h)/ih)) ||
	   !add(Vertex(v4.x, v4.y, v4.z,  x   /iw, (y+h)/ih)))
		return false;
	return true;
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr)
	{
	return draw(orientation, iptr->getTexture(), iptr->getX(), iptr->getY(), iptr->getW(), iptr->getH());
	}

bool VertexBuffer::draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr)
	{
	return draw(orientation/*-sptr.getCurrentFrame().getOffset()*/, sptr.getCurrentFrame().getImage());
	}

bool VertexBuffer::save(const std::string& path)
	{
	if(vertices.empty())
		{
		LOG_WARNING("VertexBuffer.save: Bufor jest pusty lub zamkniety");
		return false;
		}

	std::fstream out;

	out.open(path.c_str(), std::ios::out|std::ios::binary|std::ios::trunc);

	if(!out.is_open())
		{
		LOG_ERROR("VertexBuffer.save: Nie udalo sie otworzyc pliku \"%s\"", path.c_str());
		return false;
		}

	const unsigned size=vertices.size();
	out.write((char*)&size, sizeof(size));
	out.write((char*)vertices.data(), vertices.size()*sizeof(vertices[0]));

	if(out.fail())
		{
		out.close();
		LOG_ERROR("VertexBuffer.save: Wystapil problem przy zapisie do pliku \"%s\"", path.c_str());
		return false;
		}

	out.close();

	return true;
	}

bool VertexBuffer::load(const std::string& path)
	{
	clear();

	std::fstream in;

	in.open(path.c_str(), std::ios::in|std::ios::binary);

	if(!in.is_open())
		{
		LOG_ERROR("VertexBuffer.load: Nie udalo sie otworzyc pliku \"%s\"", path.c_str());
		return false;
		}

	unsigned size;

	in.read((char*)&size, sizeof(size));

	if(in.fail())
		{
		in.close();
		LOG_ERROR("VertexBuffer.load: Wystapil problem przy odczytywaniu z pliku \"%s\" (size)", path.c_str());
		return false;
		}

	LOG_INFO("VertexBuffer.load: Plik \"%s\" zawiera %u wierzcholkow", path.c_str(), size);

	if(size>100000u)
		{
		LOG_ERROR("VertexBuffer.load: Ponad 100'000 wierzcholkow wydaje sie malo prawdopodobne - przerywanie");
		in.close();
		return false;
		}

	vertices.resize(size);

	in.read((char*)vertices.data(), vertices.size()*sizeof(vertices[0]));

	if(in.fail())
		{
		in.close();
		LOG_ERROR("VertexBuffer.load: Wystapil problem przy odczytywaniu z pliku \"%s\"", path.c_str());
		return false;
		}

	in.close();

	return true;
	}
