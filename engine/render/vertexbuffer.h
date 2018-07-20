/*
 * vertexbuffer.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "vertex.h"
#include "../math/orientation.h"

namespace Engine
	{
	namespace Graphics
		{
		class ImagePtr;
		class SpritePtr;
		}

	namespace Render
		{
		class Texture;
		class TexturePtr;

		class VertexBuffer
			{
			public:
				//GLuint vaoid; // Tablica VBO
				GLuint vboid; // Tablica werteksów

				unsigned size;

				std::vector<Vertex> vertices;

				bool isFinalized() const {return vertices.empty() && size>0u;}

			public:
				VertexBuffer(): /*vaoid(0u),*/ vboid(0u), size(0u) {}
				~VertexBuffer() {}

				bool init(Vertex *vertices, unsigned size); // add(vertex)*size + finalize (bardzo możliwe, że będą tam liczne optymalizacje, bo po co wrzucać do vectora)
				bool finalize(); // Przesłanie werteksów z RAMu do grafiki
				void flush();    // Wyczyszczenie buforów
				void clear();    // Zwolnienie całej pamięci

				bool save(const std::string& path);
				bool load(const std::string& path);

				void bind() const;
				void unbind() const;

				//GLuint getVAO() const {return vaoid;}
				GLuint getVBO() const {return vboid;}
				unsigned getSize() const {return vertices.empty()?size:vertices.size();}

				bool add(const Vertex& vertex);
				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr);
				bool draw(const Math::Orientation& orientation, const TexturePtr& tptr, float x, float y, float w, float h);
				bool draw(const Math::Orientation& orientation, const Graphics::ImagePtr& iptr);
				bool draw(const Math::Orientation& orientation, const Graphics::SpritePtr& sptr);
			};
		} /* namespace Graphics */
	} /* namespace Engine */
