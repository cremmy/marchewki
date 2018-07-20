/*
 * vertex.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

namespace Engine
	{
	namespace Render
		{
		struct Vertex
			{
			explicit Vertex():
				x(0.0f), y(0.0f), z(0.0f), p1(0.0f),
				tx(0.0f), ty(0.0f), p2(0.0f), p3(0.0f),
				nx(0.0f), ny(0.0f), nz(0.0f), p4(0.0f),
				ntx(0.0f), nty(0.0f), ntz(0.0f), p5(0.0f) {}
			Vertex(float x, float y, float z, float tx, float ty):
				x(x), y(y), z(z), p1(0.0f),
				tx(tx), ty(ty), p2(0.0f), p3(0.0f),
				nx(0.0f), ny(0.0f), nz(0.0f), p4(0.0f),
				ntx(0.0f), nty(0.0f), ntz(0.0f), p5(0.0f) {}
			Vertex(float x, float y, float z, float tx, float ty, float nx, float ny, float nz, float ntx, float nty, float ntz):
				x(x), y(y), z(z), p1(0.0f),
				tx(tx), ty(ty), p2(0.0f), p3(0.0f),
				nx(nx), ny(ny), nz(nz), p4(0.0f),
				ntx(ntx), nty(nty), ntz(ntz), p5(0.0f) {}

			float   x,   y,   z, p1;
			float  tx,  ty,  p2, p3;
			float  nx,  ny,  nz, p4;
			float ntx, nty, ntz, p5;
			};
		} /* namespace Render */
	} /* namespace Engine */


