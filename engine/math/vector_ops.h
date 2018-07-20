/*
 * vector_ops.h
 *
 *  Created on: 5 sty 2018
 *      Author: crm
 */

#pragma once

#include "vector.h"

namespace Engine
	{
	namespace Math
		{
		/*****************************************************************************/
		/* Deklaracje ****************************************************************/
		/*****************************************************************************/
		template<typename T> TVector<T> VectorAdd(const TVector<T>& a, const TVector<T>& b);
		template<typename T> TVector<T> VectorSubtract(const TVector<T>& a, const TVector<T>& b);
		template<typename T> TVector<T> VectorMultiply(const TVector<T>& a, const TVector<T>& b); // Hadamard product, Schur product, entrywise product
		template<typename T> TVector<T> VectorDivide(const TVector<T>& a, const TVector<T>& b);   // jw
		template<typename T> TVector<T> VectorNormalize(const TVector<T>& a);
		template<typename T> float VectorLength(const TVector<T>& a);
		template<typename T> float VectorLengthSqr(const TVector<T>& a);

		template<typename T> float VectorDot(const TVector<T>& a, const TVector<T>& b);
		template<typename T> TVector<T> VectorCross(const TVector<T>& a, const TVector<T>& b);

		template<typename T> TVector<T> VectorFloor(const TVector<T>& a);
		template<typename T> TVector<T> VectorAbsolute(const TVector<T>& a);
		template<typename T> TVector<T> VectorMin(const TVector<T>& a, const TVector<T>& b);
		template<typename T> TVector<T> VectorMax(const TVector<T>& a, const TVector<T>& b);
		template<typename T> TVector<T> VectorClamp(const TVector<T>& min, const TVector<T>& a, const TVector<T>& max);

		template<typename T> TVector<T> VectorZero();
		template<typename T> TVector<T> VectorOne();
		template<typename T> TVector<T> VectorReplicate(T val);

		template<typename T> TVector<T> VectorMirror(const TVector<T>& a, const TVector<T>& mirror, const TVector<T>& forward, const TVector<T>& up=TVector<T>(0, 0, 1));

		template<typename T> TVector<T> VectorLerp(const TVector<T>& a, const TVector<T>& b, float p);
		template<typename T> TVector<T> VectorSlerp(const TVector<T>& a, const TVector<T>& b, float p);

		/*****************************************************************************/
		/* Operatory *****************************************************************/
		/*****************************************************************************/
		template<typename T> inline TVector<T> operator-(const TVector<T>& a) {return TVector<T>(-a.x, -a.y, -a.z, -a.w);}
		template<typename T> inline TVector<T> operator+(const TVector<T>& a, float v) {return VectorAdd(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator-(const TVector<T>& a, float v) {return VectorSubtract(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator*(const TVector<T>& a, float v) {return VectorMultiply(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator/(const TVector<T>& a, float v) {return VectorDivide(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator+(T v, const TVector<T>& a) {return VectorAdd(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator-(T v, const TVector<T>& a) {return VectorSubtract(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator*(T v, const TVector<T>& a) {return VectorMultiply(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator/(T v, const TVector<T>& a) {return VectorDivide(a, VectorReplicate(v));}
		template<typename T> inline TVector<T> operator+(const TVector<T>& a, const TVector<T>& b) {return VectorAdd(a, b);}
		template<typename T> inline TVector<T> operator-(const TVector<T>& a, const TVector<T>& b) {return VectorSubtract(a, b);}
		template<typename T> inline TVector<T> operator*(const TVector<T>& a, const TVector<T>& b) {return VectorMultiply(a, b);}
		template<typename T> inline TVector<T> operator/(const TVector<T>& a, const TVector<T>& b) {return VectorDivide(a, b);}
		template<typename T> inline float operator|(const TVector<T>& a, const TVector<T>& b) {return VectorDot(a, b);}
		template<typename T> inline TVector<T> operator^(const TVector<T>& a, const TVector<T>& b) {return VectorCross(a, b);}

		template<typename T> inline TVector<T> operator+=(TVector<T>& a, const TVector<T>& b) {return a=a+b;}
		template<typename T> inline TVector<T> operator-=(TVector<T>& a, const TVector<T>& b) {return a=a-b;}
		template<typename T> inline TVector<T> operator*=(TVector<T>& a, const TVector<T>& b) {return a=a*b;}
		template<typename T> inline TVector<T> operator*=(TVector<T>& a, float v) {return a=a*v;}
		template<typename T> inline TVector<T> operator/=(TVector<T>& a, const TVector<T>& b) {return a=a/b;}
		template<typename T> inline TVector<T> operator/=(TVector<T>& a, float v) {return a=a/v;}

		/*****************************************************************************/
		/* Definicje *****************************************************************/
		/*****************************************************************************/
		template<typename T> inline TVector<T> VectorAdd(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
			}

		template<typename T> inline TVector<T> VectorSubtract(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
			}

		template<typename T> inline TVector<T> VectorMultiply(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
			}

		template<typename T> inline TVector<T> VectorDivide(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
			}

		template<typename T> inline TVector<T> VectorNormalize(const TVector<T>& a)
			{
			const double len=sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w);

			return TVector<T>(a.x/len, a.y/len, a.z/len, a.w/len);
			}

		template<typename T> inline float VectorLength(const TVector<T>& a)
			{
			const double len=sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w);

			return len;
			}

		template<typename T> inline float VectorLengthSqr(const TVector<T>& a)
			{
			const double len=a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w;

			return len;
			}


		template<typename T> inline float VectorDot(const TVector<T>& a, const TVector<T>& b)
			{
			const double val=a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;

			return val;
			}

		template<typename T> inline TVector<T> VectorCross(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>(a.y*b.z-b.y*a.z, b.x*a.z-a.x*b.z, a.x*b.y-b.x*a.y, 0.0f);
			}


		template<typename T> inline TVector<T> VectorFloor(const TVector<T>& a)
			{
			return TVector<T>((int)a.x, (int)a.y, (int)a.z, (int)a.w);
			}

		template<typename T> inline TVector<T> VectorAbsolute(const TVector<T>& a)
			{
			return TVector<T>(fabs(a.x), fabs(a.y), fabs(a.z), fabs(a.w));
			}

		template<typename T> inline TVector<T> VectorMin(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>( (a.x<b.x)?a.x:b.x,
							(a.y<b.y)?a.y:b.y,
							(a.z<b.z)?a.z:b.z,
							(a.w<b.w)?a.w:b.w);
			}

		template<typename T> inline TVector<T> VectorMax(const TVector<T>& a, const TVector<T>& b)
			{
			return TVector<T>( (a.x>b.x)?a.x:b.x,
							(a.y>b.y)?a.y:b.y,
							(a.z>b.z)?a.z:b.z,
							(a.w>b.w)?a.w:b.w);
			}

		template<typename T> inline TVector<T> VectorClamp(const TVector<T>& min, const TVector<T>& a, const TVector<T>& max)
			{
			return VectorMin(max, VectorMax(min, a));
			}


		template<typename T> inline TVector<T> VectorZero()
			{
			return TVector<T>(0.0f, 0.0f, 0.0f, 0.0f);
			}

		template<typename T> inline TVector<T> VectorOne()
			{
			return TVector<T>(1.0f, 1.0f, 1.0f, 1.0f);
			}

		template<typename T> inline TVector<T> VectorReplicate(T v)
			{
			return TVector<T>(v, v, v, v);
			}


		template<typename T> inline TVector<T> VectorMirror(const TVector<T>& a, const TVector<T>& mirror, const TVector<T>& normal)
			{
			//TVector<T> n=forward^up;
			TVector<T> mp=normal*((mirror-a)|normal);

			return a+2.0f*mp;
			}

		template<typename T> inline TVector<T> VectorLerp(const TVector<T>& a, const TVector<T>& b, float p)
			{
			return a+(b-a)*p;
			}

		template<typename T> inline TVector<T> VectorSlerp(const TVector<T>& a, const TVector<T>& b, float p)
			{
			const float DOTP=a|b;
			const float THETA=acos(DOTP)*p;
			return (a*cos(THETA)) + VectorNormalize(b-a*DOTP)*sin(THETA);
			}
		}
	}



