/*
 * avector.h
 *
 *  Created on: 5 sty 2018
 *      Author: crm
 */

#pragma once

#define LOG_STR_VECTOR(v) "Vector " #v ": [%7.4f %7.4f %7.4f %7.4f]", v[0], v[1], v[2], v[3]
#define LOG_STR_VECTORI(v) "Vector " #v ": [%7d %7d %7d %7d]", v[0], v[1], v[2], v[3]

namespace Engine
	{
	namespace Math
		{
		template <typename T>
		struct TVector
			{
			explicit TVector(T x=0.0f, T y=0.0f, T z=0.0f, T w=0.0f):
				x(x), y(y), z(z), w(w) {}
			TVector(const TVector<T>& a, T w):
				x(a.x), y(a.y), z(a.z), w(w) {}
			TVector(const TVector<T>& a, T z, T w):
				x(a.x), y(a.y), z(z), w(w) {}
			TVector(const TVector<T>& a, T y, T z, T w):
				x(a.x), y(y), z(z), w(w) {}
			TVector(const TVector<T>& a):
				x(a.x), y(a.y), z(a.z), w(a.w) {}

			TVector<T>& operator=(const TVector<T>& a) {x=a.x; y=a.y; z=a.z; w=a.w; return *this;}
			T& operator[](int i)
				{
				switch(i)
					{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default: return w;
					}
				}
			const T& operator[](int i) const
				{
				switch(i)
					{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default: return w;
					}
				}

			T x, y, z, w;
			};

		// Typedefy
		typedef TVector<float> Vector;
		typedef TVector<float> VectorF;
		typedef TVector<int> VectorI;
		}
	}
